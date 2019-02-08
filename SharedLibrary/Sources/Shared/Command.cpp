#include "Command.hpp"

#include "TileGrid_Info.hpp"

ClientCommand::ClientCommand(ClientCommand::Code code) : 
	code(code) { }

ClientCommand::Code ClientCommand::GetCode() const { return code; }

AuthorizationClientCommand::AuthorizationClientCommand(const std::string &login, const std::string &password) :
	ClientCommand(Code::AUTH_REQ), 
	login(login), password(password) { }

RegistrationClientCommand::RegistrationClientCommand(const std::string &login, const std::string &password) :
	ClientCommand(Code::REG_REQ), 
	login(login), password(password) { }

GameListClientRequest::GameListClientRequest() : 
	ClientCommand(Code::SERVER_LIST_REQ) 
	{ }

CreateGameClientCommand::CreateGameClientCommand(const std::string &title) :
	ClientCommand(Code::CREATE_GAME), 
	title(title) { }

JoinGameClientCommand::JoinGameClientCommand(int id) : 
	ClientCommand(Code::JOIN_GAME), 
	id(id) { }

DisconnectionClientCommand::DisconnectionClientCommand() : 
	ClientCommand(Code::DISCONNECT) 
	{ }

MoveClientCommand::MoveClientCommand(uf::Direction direction) :
	ClientCommand(Code::MOVE), 
	direction(direction) { }

ClickObjectClientCommand::ClickObjectClientCommand(int id) :
    ClientCommand(Code::CLICK_OBJECT),
    id(id) { }

DropClientCommand::DropClientCommand() :
	ClientCommand(Code::DROP)
{ }

SendChatMessageClientCommand::SendChatMessageClientCommand(const std::string &message) :
	ClientCommand(Code::SEND_CHAT_MESSAGE), 
	message(message) { }

BuildClientCommand::BuildClientCommand() :
	ClientCommand(Code::BUILD) { }

GhostClientCommand::GhostClientCommand() : 
	ClientCommand(Code::GHOST) { }



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

SendChatMessageServerCommand::SendChatMessageServerCommand(std::string &message) : 
	ServerCommand(Code::SEND_CHAT_MESSAGE), 
	message(message) { }

CommandCodeErrorServerCommand::CommandCodeErrorServerCommand() : 
	ServerCommand(Code::COMMAND_CODE_ERROR) { }