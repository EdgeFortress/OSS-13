#pragma once

#include <Shared/Network/Protocol/CommandsProcessor.h>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>

namespace network {

class SyncCommandsProcessor : public network::CommandsProcessor {
protected:
	void ProcessCommand(network::protocol::Command &generalCommand) final;

private:
	void commandProcessor_AuthorizationSuccessCommand(network::protocol::server::AuthorizationSuccessCommand &command);
	void commandProcessor_AuthorizationFailedCommand(network::protocol::server::AuthorizationFailedCommand &command);
	void commandProcessor_RegistrationSuccessCommand(network::protocol::server::RegistrationSuccessCommand &command);
	void commandProcessor_RegistrationFailedCommand(network::protocol::server::RegistrationFailedCommand &command);
	void commandProcessor_GraphicsUpdateCommand(network::protocol::server::GraphicsUpdateCommand &command);
	void commandProcessor_ControlUIUpdateCommand(network::protocol::server::ControlUIUpdateCommand &command);
	void commandProcessor_OverlayUpdateCommand(network::protocol::server::OverlayUpdateCommand &command);
	void commandProcessor_OverlayResetCommand(network::protocol::server::OverlayResetCommand &command);
	void commandProcessor_OpenWindowCommand(network::protocol::server::OpenWindowCommand &command);
	void commandProcessor_OpenSpawnWindowCommand(network::protocol::server::OpenSpawnWindowCommand &command);
	void commandProcessor_UpdateSpawnWindowCommand(network::protocol::server::UpdateSpawnWindowCommand &command);
	void commandProcessor_UpdateContextMenuCommand(network::protocol::server::UpdateContextMenuCommand &command);
	void commandProcessor_UpdateWindowCommand(network::protocol::server::UpdateWindowCommand &command);
	void commandProcessor_AddChatMessageCommand(network::protocol::server::AddChatMessageCommand &command);
};

}
