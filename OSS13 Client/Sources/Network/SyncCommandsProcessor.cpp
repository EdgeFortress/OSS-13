#include "SyncCommandsProcessor.h"

#include <Client.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/UI/UI.hpp>
#include <Graphics/UI/UIModule/GameProcessUI.hpp>
#include <Graphics/TileGrid/TileGrid.hpp>
#include <Graphics/TileGrid/Object.hpp>

using namespace network;
using namespace network::protocol;

void SyncCommandsProcessor::ProcessCommand(network::protocol::Command &generalCommand) {
	using namespace network::protocol::server;

	_REGISTRATE_COMMAND_PROCESSOR(AuthorizationSuccessCommand);
	_REGISTRATE_COMMAND_PROCESSOR(AuthorizationFailedCommand);
	_REGISTRATE_COMMAND_PROCESSOR(RegistrationSuccessCommand);
	_REGISTRATE_COMMAND_PROCESSOR(RegistrationFailedCommand);
	_REGISTRATE_COMMAND_PROCESSOR(GraphicsUpdateCommand);
	_REGISTRATE_COMMAND_PROCESSOR(ControlUIUpdateCommand);
	_REGISTRATE_COMMAND_PROCESSOR(OverlayUpdateCommand);
	_REGISTRATE_COMMAND_PROCESSOR(OverlayResetCommand);
	_REGISTRATE_COMMAND_PROCESSOR(OpenWindowCommand);
	_REGISTRATE_COMMAND_PROCESSOR(OpenSpawnWindowCommand);
	_REGISTRATE_COMMAND_PROCESSOR(UpdateSpawnWindowCommand);
	_REGISTRATE_COMMAND_PROCESSOR(UpdateContextMenuCommand);
	_REGISTRATE_COMMAND_PROCESSOR(UpdateWindowCommand);
	_REGISTRATE_COMMAND_PROCESSOR(AddChatMessageCommand);

	LOGE << __FUNCTION__ << ": unknown command (ser id is 0x" << std::hex << generalCommand.SerID() << ") was not processed!";
}

void SyncCommandsProcessor::commandProcessor_AuthorizationSuccessCommand(network::protocol::server::AuthorizationSuccessCommand &) {
	AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(authUI);
	authUI->SetServerAnswer(true);
}

void SyncCommandsProcessor::commandProcessor_AuthorizationFailedCommand(network::protocol::server::AuthorizationFailedCommand &) {
	AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(authUI);
	authUI->SetServerAnswer(false);
}

void SyncCommandsProcessor::commandProcessor_RegistrationSuccessCommand(network::protocol::server::RegistrationSuccessCommand &) {
	AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(authUI);
	authUI->SetServerAnswer(true);
}

void SyncCommandsProcessor::commandProcessor_RegistrationFailedCommand(network::protocol::server::RegistrationFailedCommand &) {
	AuthUI *authUI = dynamic_cast<AuthUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(authUI);
	authUI->SetServerAnswer(false);
}

void SyncCommandsProcessor::commandProcessor_GraphicsUpdateCommand(network::protocol::server::GraphicsUpdateCommand &command) {
	GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(gameProcessUI);
	TileGrid *tileGrid = gameProcessUI->GetTileGrid();
	EXPECT(tileGrid);

	if (command.options & server::GraphicsUpdateCommand::Option::TILES_SHIFT) {
		tileGrid->ShiftBlocks(command.firstTile);

		for (auto &tileInfo : command.tilesInfo) {
			tileGrid->SetBlock(tileInfo.coords, std::make_shared<Tile>(tileGrid, std::move(tileInfo)));
		}
	}
	if (command.options & server::GraphicsUpdateCommand::Option::CAMERA_MOVE) {
		tileGrid->SetCameraPosition(command.camera);
	}
	if (command.options & server::GraphicsUpdateCommand::Option::DIFFERENCES) {
		for (auto &generalDiff : command.diffs) {
			if (auto *diff = dynamic_cast<network::protocol::AddDiff *>(generalDiff.get())) {
				auto obj = std::make_unique<Object>(std::move(diff->objectInfo));
				tileGrid->AddObject(obj.release());
				tileGrid->RelocateObject(diff->objId, diff->coords, diff->layer);
			} else if (auto *diff = dynamic_cast<network::protocol::RemoveDiff *>(generalDiff.get())) {
				tileGrid->RemoveObject(diff->objId);
			} else if (auto *diff = dynamic_cast<network::protocol::FieldsDiff *>(generalDiff.get())) {
				tileGrid->AmendObjectChanges(std::forward<network::protocol::FieldsDiff>(*diff));
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
	if (command.options & server::GraphicsUpdateCommand::Option::NEW_CONTROLLABLE) {
		tileGrid->SetControllable(command.controllableId, command.controllableSpeed);
	}
	if (command.options & server::GraphicsUpdateCommand::Option::NEW_FOV) {
		tileGrid->SetFOV(command.fov, command.fovZ);
	}
}

void SyncCommandsProcessor::commandProcessor_ControlUIUpdateCommand(network::protocol::server::ControlUIUpdateCommand &command) {
	GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(gameProcessUI);
	TileGrid *tileGrid = gameProcessUI->GetTileGrid();
	EXPECT(tileGrid);
	tileGrid->UpdateControlUI(command.elements, command.clear);
}

void SyncCommandsProcessor::commandProcessor_OverlayUpdateCommand(network::protocol::server::OverlayUpdateCommand &command) {
	GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(gameProcessUI);
	TileGrid *tileGrid = gameProcessUI->GetTileGrid();
	EXPECT(tileGrid);
	tileGrid->UpdateOverlay(command.overlayInfo);
}

void SyncCommandsProcessor::commandProcessor_OverlayResetCommand(network::protocol::server::OverlayResetCommand &) {
	GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(gameProcessUI);
	TileGrid *tileGrid = gameProcessUI->GetTileGrid();
	EXPECT(tileGrid);
	tileGrid->ResetOverlay();
}

void SyncCommandsProcessor::commandProcessor_OpenWindowCommand(network::protocol::server::OpenWindowCommand &command) {
	UIModule *uiModule = CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule();
	EXPECT(uiModule);
	uiModule->OpenWindow(command.id.c_str(), command.data);
}

void SyncCommandsProcessor::commandProcessor_OpenSpawnWindowCommand(network::protocol::server::OpenSpawnWindowCommand &) {
	GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(gameProcessUI);
	gameProcessUI->OpenSpawnWindow();
}

void SyncCommandsProcessor::commandProcessor_UpdateSpawnWindowCommand(network::protocol::server::UpdateSpawnWindowCommand &command) {
	GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(gameProcessUI);
	gameProcessUI->UpdateSpawnWindow(std::forward<std::vector<network::protocol::ObjectType>>(command.types));
}

void SyncCommandsProcessor::commandProcessor_UpdateContextMenuCommand(network::protocol::server::UpdateContextMenuCommand &command) {
	GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(gameProcessUI);
	gameProcessUI->UpdateContextMenu(std::forward<network::protocol::ContextMenuData>(command.data));
}

void SyncCommandsProcessor::commandProcessor_UpdateWindowCommand(network::protocol::server::UpdateWindowCommand &command) {
	UIModule *uiModule = CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule();
	EXPECT(uiModule);
	uiModule->UpdateWindow(command.data->window, *command.data);
}

void SyncCommandsProcessor::commandProcessor_AddChatMessageCommand(network::protocol::server::AddChatMessageCommand &command) {
	GameProcessUI *gameProcessUI = dynamic_cast<GameProcessUI *>(CC::Get()->GetWindow()->GetUI()->GetCurrentUIModule());
	EXPECT(gameProcessUI);
	gameProcessUI->Receive(command.message);
}
