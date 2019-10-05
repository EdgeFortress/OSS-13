#pragma once

#include <Shared/Network/Protocol/CommandsProcessor.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>

class Player;

class PlayerCommandsProcessor : public network::CommandsProcessor {
public:
	PlayerCommandsProcessor(Player *player);

	void ProcessCommand(network::protocol::Command &) final;

private:
	void commandProcessor_MoveCommand(network::protocol::client::MoveCommand &command);
	void commandProcessor_MoveZCommand(network::protocol::client::MoveZCommand &command);
	void commandProcessor_ClickObjectCommand(network::protocol::client::ClickObjectCommand &command);
	void commandProcessor_ClickTileCommand(network::protocol::client::ClickTileCommand &command);
	void commandProcessor_ClickControlUICommand(network::protocol::client::ClickControlUICommand &command);
	void commandProcessor_SendChatMessageCommand(network::protocol::client::SendChatMessageCommand &command);
	void commandProcessor_UIInputCommand(network::protocol::client::UIInputCommand &command);
	void commandProcessor_UITriggerCommand(network::protocol::client::UITriggerCommand &command);
	void commandProcessor_CallVerbCommand(network::protocol::client::CallVerbCommand &command);
	void commandProcessor_SpawnWindowSearchCommand(network::protocol::client::SpawnWindowSearchCommand &command);
	void commandProcessor_SpawnWindowSpawnCommand(network::protocol::client::SpawnWindowSpawnCommand &command);
	void commandProcessor_ContextMenuUpdateCommand(network::protocol::client::ContextMenuUpdateCommand &command);
	void commandProcessor_ContextMenuClickCommand(network::protocol::client::ContextMenuClickCommand &command);

private:
	Player *player;
};
