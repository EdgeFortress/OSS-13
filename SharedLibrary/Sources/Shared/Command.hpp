#pragma once

#include <string>
#include <list>

#include "Types.hpp"
#include "Geometry.hpp"

struct ClientCommand {
	enum class Code : char {
		AUTH_REQ = 1,
		REG_REQ,
		SERVER_LIST_REQ,

		CREATE_GAME,
		JOIN_GAME,
		DISCONNECT,

		MOVE,
		SEND_CHAT_MESSAGE,
		BUILD,
		GHOST
	};

	virtual Code GetCode() const final;

protected:
	explicit ClientCommand(Code code);
private:
	Code code;
};

struct AuthorizationClientCommand : public ClientCommand {
	std::string login;
	std::string password;

	AuthorizationClientCommand(const std::string &login, const std::string &password);
};

struct RegistrationClientCommand : public ClientCommand {
	std::string login;
	std::string password;

	RegistrationClientCommand(const std::string &login, const std::string &password);
};

struct GameListClientRequest : public ClientCommand {
	GameListClientRequest();
};

struct CreateGameClientCommand : public ClientCommand {
	std::string title;

	CreateGameClientCommand(const std::string &title);
};

struct JoinGameClientCommand : public ClientCommand {
	int id;

	JoinGameClientCommand(int id);
};

struct DisconnectionClientCommand : public ClientCommand {
	DisconnectionClientCommand();
};

struct MoveClientCommand : public ClientCommand {
	uf::Direction direction;

	MoveClientCommand(uf::Direction direction);
};

struct SendChatMessageClientCommand : public ClientCommand {
	std::string message;

	SendChatMessageClientCommand(std::string &message);
};

struct BuildClientCommand : public ClientCommand {
	BuildClientCommand();
};

struct GhostClientCommand : public ClientCommand {
	GhostClientCommand();
};


struct ServerCommand {
	enum class Code : char {
		AUTH_SUCCESS = 1,
		REG_SUCCESS,
		AUTH_ERROR,
		REG_ERROR,

		GAME_CREATE_SUCCESS,
		GAME_CREATE_ERROR,
		GAME_LIST,
		GAME_JOIN_SUCCESS,
		GAME_JOIN_ERROR,

		GRAPHICS_UPDATE,

		SEND_CHAT_MESSAGE,

		COMMAND_CODE_ERROR
	};

	virtual Code GetCode() const final;

protected:
	ServerCommand(Code code);
private:
	Code code;
};

struct AuthSuccessServerCommand : public ServerCommand {
	AuthSuccessServerCommand();
};

struct RegSuccessServerCommand : public ServerCommand {
	RegSuccessServerCommand();
};

struct AuthErrorServerCommand : public ServerCommand {
	AuthErrorServerCommand();
};

struct RegErrorServerCommand : public ServerCommand {
	RegErrorServerCommand();
};

struct GameCreateSuccessServerCommand : public ServerCommand {
	GameCreateSuccessServerCommand();
};

struct GameCreateErrorServerCommand : public ServerCommand {
	GameCreateErrorServerCommand();
};

struct GameJoinSuccessServerCommand : public ServerCommand {
	GameJoinSuccessServerCommand();
};

struct GameJoinErrorServerCommand : public ServerCommand {
	GameJoinErrorServerCommand();
};

struct GameListServerCommand : public ServerCommand {
	GameListServerCommand();
};

struct BlockInfo;
struct Diff;

struct GraphicsUpdateServerCommand : public ServerCommand {
	enum Option {
		EMPTY = 0,
		BLOCKS_SHIFT = 1,
		CAMERA_MOVE = 1 << 1,
		DIFFERENCES = 1 << 2,
		NEW_CONTROLLABLE = 1 << 3
	};

	std::list<sptr<Diff>> diffs;
	std::list<BlockInfo> blocksInfo;

	Option options;

	int cameraX;
	int cameraY;
	int firstBlockX;
	int firstBlockY;
	int controllable_id;
	float controllableSpeed;

	GraphicsUpdateServerCommand();
};

struct SendChatMessageServerCommand : public ServerCommand {
	std::string message;

	SendChatMessageServerCommand(std::string &message);
};

struct CommandCodeErrorServerCommand : public ServerCommand {
	CommandCodeErrorServerCommand();
};
