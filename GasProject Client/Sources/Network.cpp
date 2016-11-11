#include <thread>
#include <string>

#include <SFML/Network.hpp>

#include "Network.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/UI/UI.hpp"

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
            sf::Int32 size;
            packet >> size;
            for (int i = 1; i <= size; i++) {
                sf::Int32 id, num_of_players;
                sf::String title;
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
        case ServerCommand::Code::GRAPHICS_FULL_UPDATE:
            TileGrid &tileGrid = *CC::Get()->GetWindow()->GetTileGrid();
            tileGrid.Lock();
            packet >> tileGrid;
            tileGrid.Unlock();
            break;
    };
}

Packet &operator<<(Packet &packet, ClientCommand *command) {
    packet << sf::Int32(command->GetCode());
    switch (command->GetCode()) {
        case ClientCommand::Code::AUTH_REQ: {
            auto c = dynamic_cast<AuthorizationClientCommand *>(command);
            packet << sf::String(c->login) << sf::String(c->password);
            break;
        }
        case ClientCommand::Code::REG_REQ: {
            auto c = dynamic_cast<RegistrationClientCommand *>(command);
            packet << sf::String(c->login) << sf::String(c->password);
            break;
        }
        case ClientCommand::Code::JOIN_GAME: {
            auto c = dynamic_cast<JoinGameClientCommand *>(command);
            packet << sf::Int32(c->id);
            break;
        }
    }
    return packet;
}

Packet &operator>>(Packet &packet, TileGrid &tileGrid) {
    sf::Int32 xPos, yPos;
    packet >> xPos >> yPos;
    tileGrid.xPos = xPos;
    tileGrid.yPos = yPos;

    for (auto &vect : tileGrid.blocks)
        for (auto &block : vect)
            packet >> *block;
    return packet;
}

Packet &operator>>(Packet &packet, Block &block) {
    sf::Int32 id;
    packet >> id;
    block.id = id;
    for (auto &vect : block.tiles)
        for (auto &tile : vect)
            packet >> *tile;
    return packet;
}

Packet &operator>>(Packet &packet, Tile &tile) {
    sf::Int32 size;
    packet >> size;
    tile.Clear();
    for (int i = 0; i < size; i++) {
        Object *object = new Object();
        packet >> *object;
        tile.content.push_back(uptr<Object>(object));
    }
    return packet;
}

Packet &operator>>(Packet &packet, Object &object) {
    sf::Int32 sprite;
    packet >> sprite;
    object.SetSprite(Global::Sprite(sprite));
    return packet;
}

sf::IpAddress Connection::serverIp;
int Connection::serverPort;
Connection::Status Connection::status = Connection::Status::INACTIVE;
uptr<std::thread> Connection::thread;
sf::TcpSocket Connection::socket;
ThreadSafeQueue<ClientCommand *> Connection::commandQueue;