#include "Command.hpp"

#include "TileGrid_Info.hpp"

ServerCommand::ServerCommand(Code code) :
	code(code) { }
ServerCommand::Code ServerCommand::GetCode() const { return code; }

AuthSuccessServerCommand::AuthSuccessServerCommand() : 
	ServerCommand(Code::AUTH_SUCCESS) 
	{ }

RegSuccessServerCommand::RegSuccessServerCommand() : 
	ServerCommand(Code::REG_SUCCESS) 
	{ }

AuthErrorServerCommand::AuthErrorServerCommand() : 
	ServerCommand(Code::AUTH_ERROR) 
	{ }

RegErrorServerCommand::RegErrorServerCommand() : 
	ServerCommand(Code::REG_ERROR) 
	{ }

GameCreateSuccessServerCommand::GameCreateSuccessServerCommand() : 
	ServerCommand(Code::GAME_CREATE_SUCCESS) 
	{ }

GameCreateErrorServerCommand::GameCreateErrorServerCommand() : 
	ServerCommand(Code::GAME_CREATE_ERROR) 
	{ }

GameJoinSuccessServerCommand::GameJoinSuccessServerCommand() : 
	ServerCommand(Code::GAME_JOIN_SUCCESS) 
	{ }

GameJoinErrorServerCommand::GameJoinErrorServerCommand() : 
	ServerCommand(Code::GAME_JOIN_ERROR) 
	{ }

GameListServerCommand::GameListServerCommand() : 
	ServerCommand(Code::GAME_LIST) 
	{ }

GraphicsUpdateServerCommand::GraphicsUpdateServerCommand() : 
	ServerCommand(Code::GRAPHICS_UPDATE) 
	{ }

OverlayUpdateServerCommand::OverlayUpdateServerCommand() :
	ServerCommand(Code::OVERLAY_UPDATE) 
{ }

OpenWindowServerCommand::OpenWindowServerCommand(const std::string &layout) :
	ServerCommand(Code::OPEN_WINDOW),
	layout(layout)
{ }

SendChatMessageServerCommand::SendChatMessageServerCommand(std::string &message) : 
	ServerCommand(Code::SEND_CHAT_MESSAGE), 
	message(message) { }

CommandCodeErrorServerCommand::CommandCodeErrorServerCommand() : 
	ServerCommand(Code::COMMAND_CODE_ERROR) { }
