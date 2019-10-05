#include "PlayerCommandsProcessor.h"

#include "Player.h"

#include <Game.h>
#include <IScriptEngine.h>
#include <World/Objects/Object.hpp>
#include <World/Objects/Control.hpp>

#include <Shared/ErrorHandling.h>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>

using namespace std::string_literals;

PlayerCommandsProcessor::PlayerCommandsProcessor(Player *player) 
	: player(player)
{ }

void PlayerCommandsProcessor::ProcessCommand(network::protocol::Command &generalCommand) {
	using namespace network::protocol::client;

	_REGISTRATE_COMMAND_PROCESSOR(MoveCommand);
	_REGISTRATE_COMMAND_PROCESSOR(MoveZCommand);
	_REGISTRATE_COMMAND_PROCESSOR(ClickObjectCommand);
	_REGISTRATE_COMMAND_PROCESSOR(ClickTileCommand);
	_REGISTRATE_COMMAND_PROCESSOR(ClickControlUICommand);
	_REGISTRATE_COMMAND_PROCESSOR(SendChatMessageCommand);
	_REGISTRATE_COMMAND_PROCESSOR(UIInputCommand);
	_REGISTRATE_COMMAND_PROCESSOR(UITriggerCommand);
	_REGISTRATE_COMMAND_PROCESSOR(CallVerbCommand);
	_REGISTRATE_COMMAND_PROCESSOR(SpawnWindowSearchCommand);
	_REGISTRATE_COMMAND_PROCESSOR(SpawnWindowSpawnCommand);
	_REGISTRATE_COMMAND_PROCESSOR(ContextMenuUpdateCommand);
	_REGISTRATE_COMMAND_PROCESSOR(ContextMenuClickCommand);

	LOGE << __FUNCTION__ << ": unknown command (ser id is 0x" << std::hex << generalCommand.SerID() << ") was not processed! "
		<< "Player: " << player->GetCKey();
}

void PlayerCommandsProcessor::commandProcessor_MoveCommand(network::protocol::client::MoveCommand &command) {
	auto control = player->GetControl();
	if (control) {
		control->MoveCommand(uf::DirectionToVect(command.direction).xy());
	}
}

void PlayerCommandsProcessor::commandProcessor_MoveZCommand(network::protocol::client::MoveZCommand &command) {
	auto control = player->GetControl();
	if (control) {
		control->MoveZCommand(command.up);
	}
}

void PlayerCommandsProcessor::commandProcessor_ClickObjectCommand(network::protocol::client::ClickObjectCommand &command) {
	auto control = player->GetControl();
	if (control) {
		control->ClickObjectCommand(command.id);
	}
}

void PlayerCommandsProcessor::commandProcessor_ClickTileCommand(network::protocol::client::ClickTileCommand &command) {
	auto control = player->GetControl();
	if (control) {
		control->ClickTileCommand(player->camera->ViewCoordsToWorldCoords(command.pos));
	}
}

void PlayerCommandsProcessor::commandProcessor_ClickControlUICommand(network::protocol::client::ClickControlUICommand &command) {
	auto control = player->GetControl();
	if (control) {
		control->ClickUICommand(command.id);
	}
}

void PlayerCommandsProcessor::commandProcessor_SendChatMessageCommand(network::protocol::client::SendChatMessageCommand &command) {
	GGame->GetChat()->AddMessage("<" + player->GetCKey() + ">" + command.message);
}

void PlayerCommandsProcessor::commandProcessor_UIInputCommand(network::protocol::client::UIInputCommand &command) {
	auto iter = player->uiSinks.find(command.data->window);
	if (iter != player->uiSinks.end())
		iter->second->OnInput(command.data->handle, std::forward<std::unique_ptr<network::protocol::UIData>>(command.data));
}

void PlayerCommandsProcessor::commandProcessor_UITriggerCommand(network::protocol::client::UITriggerCommand &command) {
	auto iter = player->uiSinks.find(command.window);
	if (iter != player->uiSinks.end())
		iter->second->OnTrigger(command.trigger);
}

void PlayerCommandsProcessor::commandProcessor_CallVerbCommand(network::protocol::client::CallVerbCommand &command) {
	auto &verb = command.verb;
	try {
		auto delimiter = verb.find(".");
		EXPECT(delimiter != verb.npos);
		std::string verbHolder = verb.substr(0, delimiter);
		std::string verbName = verb.substr(delimiter + 1);

		auto nameAndVerbHolder = player->verbsHolders.find(verbHolder);
		EXPECT_WITH_MSG(nameAndVerbHolder != player->verbsHolders.end(), "VerbHolder \""s + verbHolder + "\" doesn't exist!");

		nameAndVerbHolder->second->CallVerb(player, verbName);
	} catch (const std::exception &e) {
		MANAGE_EXCEPTION(e);
	}
}

void PlayerCommandsProcessor::commandProcessor_SpawnWindowSearchCommand(network::protocol::client::SpawnWindowSearchCommand &command) {
	auto types = GGame->GetScriptEngine()->GetTypesInfo(command.searchBuffer);
	auto answer = std::make_unique<network::protocol::server::UpdateSpawnWindowCommand>();

	for (auto type : types)
		answer->types.push_back(*type);

	player->AddCommandToClient(answer.release());
}

void PlayerCommandsProcessor::commandProcessor_SpawnWindowSpawnCommand(network::protocol::client::SpawnWindowSpawnCommand &command) {
	auto control = player->GetControl();
	if (control) {
		auto *obj = GGame->GetScriptEngine()->CreateObject(command.typeKey);
		if (obj)
			obj->SetTile(control->GetOwner()->GetTile());
	}
}

void PlayerCommandsProcessor::commandProcessor_ContextMenuUpdateCommand(network::protocol::client::ContextMenuUpdateCommand &command) {
	auto camera = player->GetCamera();
	EXPECT(camera);
	camera->AskUpdateContextMenu(command.tileCoords);
}

void PlayerCommandsProcessor::commandProcessor_ContextMenuClickCommand(network::protocol::client::ContextMenuClickCommand &command) {
	auto camera = player->GetCamera();
	EXPECT(camera);
	camera->ClickContextMenu(command.node, command.verb);
}
