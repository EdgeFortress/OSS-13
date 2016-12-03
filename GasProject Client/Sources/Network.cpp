#include <thread>
#include <string>

#include <SFML/Network.hpp>

#include "Graphics/UI/UI.hpp"
#include "Graphics/UI/AuthUI.hpp"
#include "Graphics/UI/GameListUI.hpp"
#include "Graphics/UI/GameProcessUI.hpp"

#include "Network.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Common/Useful.hpp"
#include "Graphics/TileGrid/TileGrid.hpp"

using namespace std;
using namespace sf;

bool Connection::Start(string ip, int port) {
    status = Status::WAITING;

    serverIp = ip;
    serverPort = port;
    Connection::thread.reset(new std::thread(&session));

    while (GetStatus() == Status::WAITING) {
        sleep(seconds(0.01f));
    }
    if (GetStatus() == Status::CONNECTED)
        return true;
    if (GetStatus() == Status::NOT_CONNECTED)
        return false;
    return false;
}

void Connection::Stop() {
    Connection::commandQueue.Push(new DisconnectionClientCommand());
    status = Status::NOT_CONNECTED;
    thread->join();
}

void Connection::session() {
    if (socket.connect(serverIp, serverPort, seconds(5)) != sf::Socket::Done)
        status = Status::NOT_CONNECTED;
    else
        status = Status::CONNECTED;

    socket.setBlocking(false);

    while (status == Status::CONNECTED) {
        bool working = false;
        if (!commandQueue.Empty()) {
            sendCommands();
            working = true;
        }
        sf::Packet packet;
        if (socket.receive(packet) == sf::Socket::Done) {
            parsePacket(packet);
            working = true;
        }
        if (!working) sleep(seconds(0.01f));
    }
    if (!commandQueue.Empty())
        sendCommands();
}

void Connection::sendCommands() {
    while (!commandQueue.Empty()) {
        sf::Packet packet;
        ClientCommand *temp = commandQueue.Pop();
        packet << temp;
        if (temp) delete temp;
        while (socket.send(packet) == sf::Socket::Partial);
    }
}

void Connection::parsePacket(Packet &packet) {
    sf::Int32 code;
    packet >> code;
    switch (static_cast<ServerCommand::Code>(code)) {
        case ServerCommand::Code::AUTH_SUCCESS:
            CC::Get()->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(true);
            break;
        case ServerCommand::Code::REG_SUCCESS:
            CC::Get()->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(true);
            break;
        case ServerCommand::Code::AUTH_ERROR:
            CC::Get()->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(false);
            break;
        case ServerCommand::Code::REG_ERROR:
            CC::Get()->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(false);
            break;
        case ServerCommand::Code::GAME_CREATE_SUCCESS:
            break;
        case ServerCommand::Code::GAME_CREATE_ERROR:
            break;
        case ServerCommand::Code::GAME_LIST: {
            CC::Get()->GetWindow()->GetUI()->Lock();
            CC::Get()->GetWindow()->GetUI()->GetGameListUI()->Clear();
            Int32 size;
            packet >> size;
            for (int i = 1; i <= size; i++) {
                Int32 id, num_of_players;
                String title;
                packet >> id >> title >> num_of_players;
                CC::Get()->GetWindow()->GetUI()->GetGameListUI()->AddGame(id, title, num_of_players);
            }
            CC::Get()->GetWindow()->GetUI()->Unlock();
            break;
        }
        case ServerCommand::Code::GAME_JOIN_SUCCESS:
            CC::log << "You join the game" << endl;
            CC::Get()->SetState(new GameProcessState());
            break;
        case ServerCommand::Code::GAME_JOIN_ERROR:
            CC::log << "Error join the game" << endl;
            break;
        case ServerCommand::Code::GRAPHICS_UPDATE: {
            TileGrid &tileGrid = *CC::Get()->GetWindow()->GetTileGrid();
            Int32 options;
            packet >> options;
            tileGrid.LockDrawing();
            if (options & GraphicsUpdateServerCommand::Option::BLOCKS_SHIFT) {
                Int32 x, y, blockNum;
                packet >> x >> y >> blockNum;
                tileGrid.ShiftBlocks(x, y);
                while (blockNum) {
                    packet >> x >> y;
                    Block *block = new Block(&tileGrid);
                    packet >> *(block);
                    tileGrid.SetBlock(x, y, block);
                    blockNum--;
                }
            }
            if (options & GraphicsUpdateServerCommand::Option::CAMERA_MOVE) {
                Int32 x, y;
                packet >> x >> y;
                tileGrid.SetCameraPosition(x, y);
            }
            if (options & GraphicsUpdateServerCommand::Option::DIFFERENCES) {
                Int32 count;
                packet >> count;
                for (int i = 0; i < count; i++) {
                    Int32 type, blockX, blockY, x, y, objectNum;
                    packet >> type >> blockX >> blockY >> x >> y >> objectNum;
                    switch (Diff::Type(type)) {
                        case Diff::Type::MOVE: {
                            Int32 toX, toY, toObjectNum;
                            packet >> toX >> toY >> toObjectNum;
                            tileGrid.Move(blockX, blockY, x, y, objectNum, toX, toY, toObjectNum);
                            break;
                        }
                        case Diff::Type::ADD: {
                            Int32 sprite;
                            String name;
                            packet >> sprite >> name;
                            tileGrid.Add(blockX, blockY, x, y, objectNum, Global::Sprite(sprite), name);
                            break;
                        }
                        case Diff::Type::REMOVE: {
                            tileGrid.Remove(blockX, blockY, x, y, objectNum);
                            break;
                        }
                        default:
                            CC::log << "Wrong diff type: " << type << endl;
                            break;
                    }
                }
            }
            tileGrid.UnlockDrawing();
            break;
        }
        case ServerCommand::Code::SEND_CHAT_MESSAGE: {
            std::vector<std::wstring> message;
            sf::Int32 size;
            std::wstring str;
            std::string playerName;
            packet >> playerName;
            packet >> size;
            for (int i = 0; i < int(size); i++)
                packet >> str, message.push_back(str);
            auto chat = CC::Get()->GetWindow()->GetUI()->GetGameProcessUI()->GetChat();
            chat->AddIncomingMessage(message, playerName);
            break;
        }
    };
}

Packet &operator<<(Packet &packet, ClientCommand *command) {
    packet << Int32(command->GetCode());
    switch (command->GetCode()) {
        case ClientCommand::Code::AUTH_REQ: {
            auto c = dynamic_cast<AuthorizationClientCommand *>(command);
            packet << String(c->login) << String(c->password);
            break;
        }
        case ClientCommand::Code::REG_REQ: {
            auto c = dynamic_cast<RegistrationClientCommand *>(command);
            packet << String(c->login) << String(c->password);
            break;
        }
        case ClientCommand::Code::JOIN_GAME: {
            auto c = dynamic_cast<JoinGameClientCommand *>(command);
            packet << Int32(c->id);
            break;
        }
        case ClientCommand::Code::SEND_CHAT_MESSAGE: {
            auto c = dynamic_cast<SendChatMessageClientCommand *>(command);
            packet << sf::Int32(c->message.size());
            for (auto &str : c->message) {
                packet << str;
            }
            break;
        }
    }
    return packet;
}

Packet &operator>>(Packet &packet, TileGrid &tileGrid) {
    Int32 xPos, yPos;
    packet >> xPos >> yPos;
    tileGrid.xRelPos = xPos;
    tileGrid.yRelPos = yPos;

    for (auto &vect : tileGrid.blocks)
        for (auto &block : vect) {
            sf::Int32 id;
            packet >> id;
            if (id >= 0)
                packet >> *block;
            //else block->Clear();
            //block-> = id;
        }
    return packet;
}

Packet &operator>>(Packet &packet, Block &block) {
    //sf::Int32 id;
    //packet >> id;
    //block.id = id;
    for (auto &vect : block.tiles)
        for (auto &tile : vect)
            packet >> *tile;
    return packet;
}

Packet &operator>>(Packet &packet, Tile &tile) {
    sf::Int32 size, sprite;
    packet >> size >> sprite;
    tile.Clear();
    for (auto &spr : CC::Get()->GetWindow()->GetSprites())
        if (spr->GetKey() == Global::Sprite(sprite)) {
            tile.sprite = spr.get();
            break;
        }
    for (int i = 0; i < size; i++) {
        Object *object = new Object();
        packet >> *object;
        tile.content.push_back(uptr<Object>(object));
    }
    return packet;
}

Packet &operator>>(Packet &packet, Object &object) {
    sf::Int32 sprite;
    sf::String name;
    packet >> sprite >> name;
    object.SetSprite(Global::Sprite(sprite));
    object.name = name;
    return packet;
}

sf::IpAddress Connection::serverIp;
int Connection::serverPort;
Connection::Status Connection::status = Connection::Status::INACTIVE;
uptr<std::thread> Connection::thread;
sf::TcpSocket Connection::socket;
ThreadSafeQueue<ClientCommand *> Connection::commandQueue;