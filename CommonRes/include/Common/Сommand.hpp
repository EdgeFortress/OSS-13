#pragma once

#include <string>
#include <list>

#include "Useful.hpp"
#include "Differences.hpp"

using std::string;

struct ClientCommand {
    enum class Code : char {
        AUTH_REQ = 1,
        REG_REQ,
        SERVER_LIST_REQ,

        CREATE_GAME,
        JOIN_GAME,
        DISCONNECT,

        NORTH,
        SOUTH,
        EAST,
        WEST,

        SEND_CHAT_MESSAGE
    };

    virtual const Code GetCode() const = 0;
};

struct AuthorizationClientCommand : public ClientCommand {
    string login;
    string password;

    virtual const Code GetCode() const override { return Code::AUTH_REQ; }

    AuthorizationClientCommand(string login, string password) : login(login),
                                                                password(password) { }
};

struct RegistrationClientCommand : public ClientCommand {
    string login;
    string password;

    virtual const Code GetCode() const override { return Code::REG_REQ; }

    RegistrationClientCommand(string login, string password) : login(login),
                                                               password(password) { }
};

struct GameListClientRequest : public ClientCommand {
    virtual const Code GetCode() const override { return Code::SERVER_LIST_REQ; }
};

struct CreateGameClientCommand : public ClientCommand {
    string title;

    virtual const Code GetCode() const override { return Code::CREATE_GAME; }

    CreateGameClientCommand(string title) : title(title) { }
};

struct JoinGameClientCommand : public ClientCommand {
    int id;

    virtual const Code GetCode() const override { return Code::JOIN_GAME; }

    JoinGameClientCommand(int id) : id(id) { }
};

struct DisconnectionClientCommand : public ClientCommand {
    virtual const Code GetCode() const override { return Code::DISCONNECT; }

    DisconnectionClientCommand() { }
};

struct NorthClientCommand : public ClientCommand {
    virtual const Code GetCode() const override { return Code::NORTH; }

    NorthClientCommand() { }
};

struct SouthClientCommand : public ClientCommand {
    virtual const Code GetCode() const override { return Code::SOUTH; }

    SouthClientCommand() { }
};

struct EastClientCommand : public ClientCommand {
    virtual const Code GetCode() const override { return Code::EAST; }

    EastClientCommand() { }
};

struct WestClientCommand : public ClientCommand {
    virtual const Code GetCode() const override { return Code::WEST; }

    WestClientCommand() { }
};

struct SendChatMessageClientCommand : public ClientCommand {
    std::vector<std::wstring> message;

    virtual const Code GetCode() const override { return Code::SEND_CHAT_MESSAGE; }

    SendChatMessageClientCommand(const std::vector<std::wstring> &message) : message(message) { }
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
        //CONNECTION_ERROR,
    };

    virtual const Code GetCode() const = 0;
};

struct AuthSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::AUTH_SUCCESS; }
};

struct RegSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::REG_SUCCESS; }
};

struct AuthErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::AUTH_ERROR; }
};

struct RegErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::REG_ERROR; }
};

struct GameCreateSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_CREATE_SUCCESS; }
};

struct GameCreateErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_CREATE_ERROR; }
};

struct GameJoinSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_JOIN_SUCCESS; }
};

struct GameJoinErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_JOIN_ERROR; }
};

struct GameListServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_LIST; }
};

struct BlockInfo;

struct GraphicsUpdateServerCommand : public ServerCommand {
    enum Option {
        EMPTY = 0,
        BLOCKS_SHIFT = 1,
        CAMERA_MOVE = 1 << 1,
        DIFFERENCES = 1 << 2
    };

    std::list<sptr<Diff>> diffs;
    std::list<BlockInfo> blocksInfo;

    Option options;

    int cameraX;
    int cameraY;
    int firstBlockX;
    int firstBlockY;

    virtual const Code GetCode() const override { return Code::GRAPHICS_UPDATE; }
};

struct SendChatMessageServerCommand : public ServerCommand {
    std::vector<std::wstring> message;
    std::string playerName;

    virtual const Code GetCode() const override { return Code::SEND_CHAT_MESSAGE; }

    SendChatMessageServerCommand(const std::vector<std::wstring> &message, std::string &playerName) : message(message), playerName(playerName) { }
};

struct CommandCodeErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::COMMAND_CODE_ERROR; }
};

//struct ConnectionErrorServerCommand : public ServerCommand {
//    virtual const Code GetCode() const override { return Code::CONNECTION_ERROR; }
//};

