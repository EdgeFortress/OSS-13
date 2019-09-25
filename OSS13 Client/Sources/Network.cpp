#include "Network.hpp"

#include <thread>
#include <string>
#include <memory>

#include <SFML/Network.hpp>

#include <Client.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/TileGrid/TileGrid.hpp>
#include <Graphics/TileGrid/Object.hpp>
#include <Graphics/UI/UI.hpp>
#include <Graphics/UI/UIModule/AuthUI.hpp>
#include <Graphics/UI/UIModule/GameProcessUI.hpp>

#include <Shared/ErrorHandling.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>
#include <Shared/Network/Protocol/ServerToClient/WindowData.h>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

using namespace std;
using namespace std::string_literals;
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
			try {
				parsePacket(packet);
			} catch (const std::exception &e) {
				MANAGE_EXCEPTION(e);
			}
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

	object->id = objectInfo.id;
	object->name = objectInfo.name;
	object->layer = objectInfo.layer;
	object->direction = objectInfo.direction;
	object->density = objectInfo.density;
	object->solidity = objectInfo.solidity;
	object->opacity = objectInfo.opacity;
	object->moveSpeed = objectInfo.moveSpeed;
	object->speed = objectInfo.speed;

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

	if (auto *command = dynamic_cast<server::GameJoinSuccessCommand *>(p.get())) {
		return true;
	}
	if (auto *command = dynamic_cast<server::GameJoinErrorCommand *>(p.get())) {
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
				};
			}
		}
		if (command->options & server::GraphicsUpdateCommand::Option::NEW_CONTROLLABLE) {
			tileGrid->SetControllable(command->controllableId, command->controllableSpeed);
		}
		if (command->options & server::GraphicsUpdateCommand::Option::NEW_FOV) {
			tileGrid->SetFOV(command->fov, command->fovZ);
		}
		tileGrid->UnlockDrawing();
		return true;
	}

	if (auto *command = dynamic_cast<server::ControlUIUpdateCommand *>(p.get())) {
		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(gameProcessUI);
		TileGrid *tileGrid = gameProcessUI->GetTileGrid();
		EXPECT(tileGrid);
		tileGrid->LockDrawing();
		tileGrid->UpdateControlUI(command->elements, command->clear);
		tileGrid->UnlockDrawing();
		return true;
	}

	if (auto *command = dynamic_cast<server::OverlayUpdateCommand *>(p.get())) {
		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(gameProcessUI);
		TileGrid *tileGrid = gameProcessUI->GetTileGrid();
		EXPECT(tileGrid);
		tileGrid->LockDrawing();
		tileGrid->UpdateOverlay(command->overlayInfo);
		tileGrid->UnlockDrawing();
		return true;
	}

	if (auto *command = dynamic_cast<server::OverlayResetCommand *>(p.get())) {
		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(gameProcessUI);
		TileGrid *tileGrid = gameProcessUI->GetTileGrid();
		EXPECT(tileGrid);
		tileGrid->LockDrawing();
		tileGrid->ResetOverlay();
		tileGrid->UnlockDrawing();
		return true;
	}

	if (auto *command = dynamic_cast<server::OpenWindowCommand *>(p.get())) {
		UIModule *uiModule = CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule();
		EXPECT(uiModule);
		uiModule->OpenWindow(command->id.c_str(), command->data);
		return true;
	}

	if (auto *command = dynamic_cast<server::OpenSpawnWindowCommand *>(p.get())) {
		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(gameProcessUI);
		gameProcessUI->OpenSpawnWindow();
		return true;
	}

	if (auto *command = dynamic_cast<server::UpdateSpawnWindowCommand *>(p.get())) {
		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(gameProcessUI);
		gameProcessUI->UpdateSpawnWindow(std::forward<std::vector<network::protocol::ObjectType>>(command->types));
		return true;
	}

	if (auto *command = dynamic_cast<server::UpdateContextMenuCommand *>(p.get())) {
		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(gameProcessUI);
		gameProcessUI->UpdateContextMenu(std::forward<network::protocol::ContextMenuData>(command->data));
		return true;
	}

	if (auto *command = dynamic_cast<server::UpdateWindowCommand *>(p.get())) {
		UIModule *uiModule = CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule();
		EXPECT(uiModule);
		uiModule->UpdateWindow(command->data->window, *command->data);
		return true;
	}

	if (auto *command = dynamic_cast<server::AddChatMessageCommand *>(p.get())) {
		GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
		EXPECT(gameProcessUI);
		gameProcessUI->Receive(command->message);
		return true;
	}

	EXPECT_WITH_MSG(false, "Unknown command received! Serializable ID is "s + std::to_string(p->Id()));

	return false;
}

sf::IpAddress Connection::serverIp;
int Connection::serverPort;
Connection::Status Connection::status = Connection::Status::INACTIVE;
uptr<std::thread> Connection::thread;
sf::TcpSocket Connection::socket;
uf::ThreadSafeQueue<Command *> Connection::commandQueue;