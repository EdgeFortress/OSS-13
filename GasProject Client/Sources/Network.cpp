#include "Network.hpp"

#include <thread>
#include <string>
#include <memory>

#include <SFML/Network.hpp>

#include "Graphics/UI/UI.hpp"
#include "Graphics/UI/UIModule/AuthUI.hpp"
#include "Graphics/UI/UIModule/GameProcessUI.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid.hpp"

#include <Shared/Command.hpp>
#include <Shared/ErrorHandling.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>
#include <Shared/Network/Protocol/ServerToClient/WindowData.h>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

using namespace std;
using namespace sf;
using namespace network::protocol;

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
    Connection::commandQueue.Push(new client::DisconnectionCommand());
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
		uf::InputArchive ar(packet);
        Command *temp = commandQueue.Pop();
        ar << *temp;
        if (temp) delete temp;
        while (socket.send(packet) == sf::Socket::Partial);
    }
}

std::unique_ptr<Object> CreateObjectWithInfo(const network::protocol::ObjectInfo &objectInfo) {
	auto object = std::make_unique<Object>();
	
	for (auto &sprite : objectInfo.spriteIds) {
		object->AddSprite(uint(sprite));
	}
	
	object->name = objectInfo.name;
	object->layer = objectInfo.layer;
	object->direction = objectInfo.direction;
	object->dense = objectInfo.dense;
	object->moveSpeed = objectInfo.moveSpeed;
	object->constSpeed = objectInfo.constSpeed;

	return object;
}

std::unique_ptr<Tile> CreateTileWithInfo(TileGrid *tileGrid, const network::protocol::TileInfo &tileInfo) {
	auto tile = std::make_unique<Tile>(tileGrid);

	tile->sprite = CC::Get()->RM.CreateSprite(uint(tileInfo.sprite));

	for (auto &objInfo : tileInfo.content) {
		auto &objects = tile->GetTileGrid()->objects;

		auto iter = objects.find(objInfo.id);
		if (iter == objects.end()) {
			objects[objInfo.id] = CreateObjectWithInfo(objInfo);
		}
		objects[objInfo.id]->SetID(objInfo.id);

		tile->AddObject(objects[objInfo.id].get());
	}
	tile->Resize(tile->GetTileGrid()->GetTileSize());
	return tile;
}

bool Connection::parsePacket(Packet &packet) {
	uf::OutputArchive ar(packet);
	auto p = ar.UnpackSerializable();

	if (auto *command = dynamic_cast<server::AuthorizationSuccessCommand *>(p.get())) {
		AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(authUI);
		authUI->SetServerAnswer(true);
		return true;
	}

	if (auto *command = dynamic_cast<server::AuthorizationFailedCommand *>(p.get())) {
		AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(authUI);
		authUI->SetServerAnswer(false);
		return true;
	}

	if (auto *command = dynamic_cast<server::RegistrationSuccessCommand *>(p.get())) {
		AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(authUI);
		authUI->SetServerAnswer(true);
		return true;
	}

	if (auto *command = dynamic_cast<server::RegistrationFailedCommand *>(p.get())) {
		AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(authUI);
		authUI->SetServerAnswer(false);
		return true;
	}

	if (auto *command = dynamic_cast<server::GraphicsUpdateCommand *>(p.get())) {
		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
        EXPECT(gameProcessUI);
        TileGrid *tileGrid = gameProcessUI->GetTileGrid();
		EXPECT(tileGrid);
        tileGrid->LockDrawing();
        if (command->options & server::GraphicsUpdateCommand::Option::TILES_SHIFT) {
            tileGrid->ShiftBlocks(command->firstTile);

			for (auto &tileInfo : command->tilesInfo) {
				tileGrid->SetBlock(tileInfo.coords, CreateTileWithInfo(tileGrid, tileInfo));
			}
        }
		if (command->options & server::GraphicsUpdateCommand::Option::CAMERA_MOVE) {
            tileGrid->SetCameraPosition(command->camera);
        }
        if (command->options & server::GraphicsUpdateCommand::Option::DIFFERENCES) {
			for (auto &generalDiff : command->diffs) {
				LOGI << typeid(*generalDiff).name();
				if (auto *diff = dynamic_cast<network::protocol::AddDiff *>(generalDiff.get())) {
					auto obj = CreateObjectWithInfo(diff->objectInfo);
					tileGrid->AddObject(obj.release());
					tileGrid->RelocateObject(diff->objId, diff->coords, diff->layer);
				} else if (auto *diff = dynamic_cast<network::protocol::RemoveDiff *>(generalDiff.get())) {
					tileGrid->RemoveObject(diff->objId);
				} else if (auto *diff = dynamic_cast<network::protocol::RelocateDiff *>(generalDiff.get())) {
					tileGrid->RelocateObject(diff->objId, diff->newCoords, diff->layer);
				} else if (auto *diff = dynamic_cast<network::protocol::MoveIntentDiff *>(generalDiff.get())) {
					tileGrid->SetMoveIntentObject(diff->objId, diff->direction);
				} else if (auto *diff = dynamic_cast<network::protocol::MoveDiff *>(generalDiff.get())) {
					tileGrid->MoveObject(diff->objId, diff->direction, diff->speed);
				} else if (auto *diff = dynamic_cast<network::protocol::UpdateIconsDiff *>(generalDiff.get())) {
					tileGrid->UpdateObjectIcons(diff->objId, diff->iconsIds);
				} else if (auto *diff = dynamic_cast<network::protocol::PlayAnimationDiff *>(generalDiff.get())) {
					tileGrid->PlayAnimation(diff->objId, diff->animationId);
				} else if (auto *diff = dynamic_cast<network::protocol::ChangeDirectionDiff *>(generalDiff.get())) {
					tileGrid->ChangeObjectDirection(diff->objId, diff->direction);
				} else if (auto *diff = dynamic_cast<network::protocol::StunnedDiff *>(generalDiff.get())) {
					tileGrid->Stunned(diff->objId, sf::microseconds(diff->duration.count()));
				}
			}
		}
		if (command->options & GraphicsUpdateServerCommand::Option::NEW_CONTROLLABLE) {
			tileGrid->SetControllable(command->controllableId, command->controllableSpeed);
		}
		tileGrid->UnlockDrawing();
		return true;
	}

	//	case ServerCommand::Code::OVERLAY_UPDATE: {
	//		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	//		if (!gameProcessUI) break;
	//		TileGrid *tileGrid = gameProcessUI->GetTileGrid();
	//		tileGrid->LockDrawing();
	//		tileGrid->UpdateOverlay(packet);
	//		tileGrid->UnlockDrawing();
	//		break;
	//	}
	//	case ServerCommand::Code::OVERLAY_RESET:
	//	{
	//		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	//		if (!gameProcessUI) break;
	//		TileGrid *tileGrid = gameProcessUI->GetTileGrid();
	//		tileGrid->LockDrawing();
	//		tileGrid->ResetOverlay();
	//		tileGrid->UnlockDrawing();
	//		break;
	//	}
	//	case ServerCommand::Code::OPEN_WINDOW: {
	//		uf::OutputArchive ar(packet);
	//		std::string id;
	//		ar >> id;
	//		network::protocol::WindowData data;
	//		ar >> data;
	//		UIModule *uiModule = CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule();
	//		uiModule->OpenWindow(id.c_str(), std::move(data));
	//		break;
	//	}
	//	case ServerCommand::Code::UPDATE_WINDOW: {
	//		uf::OutputArchive ar(packet);
	//		auto ser = ar.UnpackSerializable();
	//		auto *data = dynamic_cast<UIData *>(ser.get());
	//		UIModule *uiModule = CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule();
	//		uiModule->UpdateWindow(data->window, *data);
	//		break;
	//	}
 //       case ServerCommand::Code::SEND_CHAT_MESSAGE: {
 //           std::string message;
 //           packet >> message;
	//		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	//		if (gameProcessUI) gameProcessUI->Receive(message);
 //           break;
 //       }
 //   };
	return false;
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
uf::ThreadSafeQueue<Command *> Connection::commandQueue;