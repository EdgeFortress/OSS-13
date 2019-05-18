#pragma once

#include <string>
#include <list>
#include <vector>

#include <Shared/Network/Protocol/OverlayInfo.h>
#include <Shared/Network/Protocol/InputData.h>
#include <Shared/Network/Protocol/WindowData.h>

#include "Types.hpp"

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
		OVERLAY_UPDATE,
		OVERLAY_RESET,

		OPEN_WINDOW,
		UPDATE_WINDOW,

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

struct TileInfo;
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
	std::list<TileInfo> blocksInfo;

	Option options;

	int cameraX;
	int cameraY;
	int cameraZ;
	int firstBlockX;
	int firstBlockY;
	int firstBlockZ;
	int controllable_id;
	float controllableSpeed;

	GraphicsUpdateServerCommand();
};

struct OverlayUpdateServerCommand : public ServerCommand {
	std::vector<network::protocol::OverlayInfo> overlayInfo;

	OverlayUpdateServerCommand();
};

struct OverlayResetServerCommand : public ServerCommand { 
	OverlayResetServerCommand();
};

struct OpenWindowServerCommand : public ServerCommand {
	std::string id;
	network::protocol::WindowData data;

	OpenWindowServerCommand(const std::string &id, network::protocol::WindowData &&data);
};

struct UpdateWindowServerCommand : public ServerCommand {
	uptr<network::protocol::UIData> data;

	UpdateWindowServerCommand(uptr<network::protocol::UIData> &&data);
};

struct SendChatMessageServerCommand : public ServerCommand {
	std::string message;

	SendChatMessageServerCommand(std::string &message);
};

struct CommandCodeErrorServerCommand : public ServerCommand {
	CommandCodeErrorServerCommand();
};
