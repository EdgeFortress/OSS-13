#include "Network.hpp"

#include <thread>
#include <string>

#include <SFML/Network.hpp>

#include "Graphics/UI/UI.hpp"
#include "Graphics/UI/UIModule/AuthUI.hpp"
#include "Graphics/UI/UIModule/GameListUI.hpp"
#include "Graphics/UI/UIModule/GameProcessUI.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid.hpp"

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
	case ServerCommand::Code::AUTH_SUCCESS: {
		AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		if (!authUI) break;
		authUI->SetServerAnswer(true);
            break;
	}
	case ServerCommand::Code::REG_SUCCESS: {
		AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		if (!authUI) break;
		authUI->SetServerAnswer(true);
            break;
	}
	case ServerCommand::Code::AUTH_ERROR: {
		AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		if (!authUI) break;
		authUI->SetServerAnswer(false);
            break;
	}
	case ServerCommand::Code::REG_ERROR: {
		AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		if (!authUI) break;
		authUI->SetServerAnswer(false);
            break;
	}
        case ServerCommand::Code::GAME_CREATE_SUCCESS:
            break;
        case ServerCommand::Code::GAME_CREATE_ERROR:
            break;
		case ServerCommand::Code::GAME_LIST: {
			GameListUI *gameListUI = dynamic_cast<GameListUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
			if (!gameListUI) break;
			CC::Get()->GetWindow()->GetUI()->Lock();
			gameListUI->Clear();
			Int32 size;
			packet >> size;
			for (int i = 1; i <= size; i++) {
				Int32 id, num_of_players;
				String title;
				packet >> id >> title >> num_of_players;
				gameListUI->AddGame(id, title.toAnsiString(), num_of_players);
			}
			CC::Get()->GetWindow()->GetUI()->Unlock();
			break;
		}
        case ServerCommand::Code::GAME_JOIN_SUCCESS:
            LOGI << "You join the game";
            break;
        case ServerCommand::Code::GAME_JOIN_ERROR:
            LOGE << "Fail to join the game";
            break;
        case ServerCommand::Code::GRAPHICS_UPDATE:
        {
            GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
            if (!gameProcessUI) break;
            TileGrid *tileGrid = gameProcessUI->GetTileGrid();
            Int32 options;
            packet >> options;
            tileGrid->LockDrawing();
            if (options & GraphicsUpdateServerCommand::Option::BLOCKS_SHIFT) {
                Int32 x, y, z, numOfBlocks;
                packet >> x >> y >> z >> numOfBlocks;

                tileGrid->ShiftBlocks(apos(x, y, z));

                while (numOfBlocks) {
                    packet >> x >> y >> z;
                    Tile *block = new Tile(tileGrid);
                    packet >> *(block);

                    tileGrid->SetBlock(apos(x, y, z), block);

                    numOfBlocks--;
                }
            }
            if (options & GraphicsUpdateServerCommand::Option::CAMERA_MOVE) {
                Int32 x, y, z;
                packet >> x >> y >> z;

                tileGrid->SetCameraPosition(apos(x, y, z));

            }
            if (options & GraphicsUpdateServerCommand::Option::DIFFERENCES) {
                Int32 count;
                packet >> count;
                for (int i = 0; i < count; i++) {
                    Int32 type;
                    packet >> type;
                    switch (Global::DiffType(type)) {
                        case Global::DiffType::ADD:
                        {
                            Int32 id;
                            Object *object = new Object;
                            packet >> id;
                            object->SetID(id);
                            packet >> *object;

                            tileGrid->AddObject(object);

                            break;
                        }
                        case Global::DiffType::REMOVE:
                        {
                            Int32 id;
                            packet >> id;
                            tileGrid->RemoveObject(id);

                            break;
                        }
                        case Global::DiffType::RELOCATE:
                        {
                            Int32 id;
                            packet >> id;
                            Int32 toX, toY, toZ, toObjectNum;
                            packet >> toX >> toY >> toZ >> toObjectNum;

                            tileGrid->RelocateObject(id, apos(toX, toY, toZ), toObjectNum);

                            break;
                        }
                        case Global::DiffType::MOVE_INTENT:
                        {
                            Int32 id;
                            packet >> id;
                            Int8 direction;
                            packet >> direction;

                            tileGrid->SetMoveIntentObject(id, uf::Direction(direction));

                            break;
                        }
                        case Global::DiffType::MOVE:
                        {
                            Int32 id;
                            packet >> id;
                            Int8 direction;
                            packet >> direction;
                            float speed;
                            packet >> speed;

                            tileGrid->MoveObject(id, uf::Direction(direction));

                            break;
                        }
						case Global::DiffType::UPDATE_ICONS:
						{
							Int32 id, spriteNum;
							packet >> id >> spriteNum;
							std::vector<uint32_t> sprites;
							while (spriteNum--) {
								Int32 sprite;
								packet >> sprite;
								sprites.push_back(sprite);
							}
							tileGrid->UpdateObjectIcons(id, sprites);
							break;
						}
                        case Global::DiffType::PLAY_ANIMATION:
                        {
                            Int32 id, sprite;
                            packet >> id >> sprite;
                            tileGrid->PlayAnimation(id, sprite);
                            break;
                        }
                        case Global::DiffType::CHANGE_DIRECTION:
                        {
                            Int32 id;
                            packet >> id;
                            Int8 direction;
                            packet >> direction;

                            tileGrid->ChangeObjectDirection(id, uf::Direction(direction));

                            break;
                        }
						case Global::DiffType::STUNNED:
							Int32 id, duration;
							packet >> id >> duration;

							tileGrid->Stunned(id, sf::milliseconds(duration));

							break;
                        default:
                            LOGE << "Wrong diff type: " << type;
                            break;
                    }
                }
            }
            if (options & GraphicsUpdateServerCommand::Option::NEW_CONTROLLABLE) {
                Int32 id;
                float speed;
                packet >> id >> speed;
                tileGrid->SetControllable(id, speed);
            }
            tileGrid->UnlockDrawing();
            break;
        }
		case ServerCommand::Code::OVERLAY_UPDATE: {
			GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
			if (!gameProcessUI) break;
			TileGrid *tileGrid = gameProcessUI->GetTileGrid();
			tileGrid->LockDrawing();
			tileGrid->UpdateOverlay(packet);
			tileGrid->UnlockDrawing();
			break;
		}
		case ServerCommand::Code::OPEN_WINDOW: {
			std::string layout;
			packet >> layout;
			UIModule *uiModule = CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule();
			uiModule->OpenWindow(layout.c_str());
			break;
		}
        case ServerCommand::Code::SEND_CHAT_MESSAGE: {
            std::string message;
            packet >> message;
			GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
			if (gameProcessUI) gameProcessUI->Receive(message);
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
        case ClientCommand::Code::MOVE: {
            auto c = dynamic_cast<MoveClientCommand *>(command);
            packet << sf::Int8(c->direction);
            break;
        }
        case ClientCommand::Code::MOVEZ: {
            auto c = dynamic_cast<MoveZClientCommand *>(command);
            packet << c->up;
            break;
        }
        case ClientCommand::Code::CLICK_OBJECT: {
            auto c = dynamic_cast<ClickObjectClientCommand *>(command);
            packet << sf::Int32(c->id);
            break;
        }
        case ClientCommand::Code::SEND_CHAT_MESSAGE: {
            auto c = dynamic_cast<SendChatMessageClientCommand *>(command);
            packet << c->message;
            break;
        }
    }
    return packet;
}

Packet &operator>>(Packet &packet, TileGrid &tileGrid) {
    Int32 xRelPos, yRelPos;
    packet >> xRelPos >> yRelPos;
    tileGrid.cameraRelPos.x = xRelPos;
    tileGrid.cameraRelPos.y = yRelPos;

    for (auto &block : tileGrid.blocks) {
		sf::Int32 id;
		packet >> id;
		if (id >= 0)
			packet >> *block;
	}
    return packet;
}

Packet &operator>>(Packet &packet, Tile &tile) {
    sf::Int32 size, sprite;
    packet >> size >> sprite;
    tile.Clear();
    tile.sprite = CC::Get()->RM.CreateSprite(uint(sprite));
    for (int i = 0; i < size; i++) {
        sf::Int32 id;
        packet >> id;

        auto &objects = tile.GetTileGrid()->objects;

        auto iter = objects.find(id);
        if (iter == objects.end()) {
            objects[id] = std::make_unique<Object>();
        }
        objects[id]->SetID(id);
        packet >> *(objects[id]);

        tile.AddObject(objects[id].get());
    }
    tile.Resize(tile.GetTileGrid()->GetTileSize());
    return packet;
}

Packet &operator>>(Packet &packet, Object &object) {
    sf::Int32 spriteNum, layer;
	sf::Int8 direction;
    sf::String name;
	bool dense;
    float moveSpeed;
    uf::vec2f constSpeed;

    packet >> spriteNum;
	object.ClearSprites();
	while (spriteNum--) {
		sf::Int32 spriteId;
		packet >> spriteId;
		object.AddSprite(uint(spriteId));
	}

	packet >> name >> layer >> direction >> dense;
    packet >> moveSpeed >> constSpeed.x >> constSpeed.y;
    object.name = name.toAnsiString();
    object.layer = layer;
	object.direction = uf::Direction(direction);
	object.dense = dense;

    object.moveSpeed = moveSpeed;
    object.constSpeed = constSpeed;
    
    return packet;
}

sf::IpAddress Connection::serverIp;
int Connection::serverPort;
Connection::Status Connection::status = Connection::Status::INACTIVE;
uptr<std::thread> Connection::thread;
sf::TcpSocket Connection::socket;
uf::ThreadSafeQueue<ClientCommand *> Connection::commandQueue;