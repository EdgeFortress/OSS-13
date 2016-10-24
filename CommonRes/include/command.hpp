#pragma once

#include <string>
#include <list>

#include <useful.hpp>

using std::string;

class Game;

struct ClientCommand {
    enum Code {
        AUTH_REQ = 1,
        REG_REQ,
        SERVER_LIST_REQ,
        CREATE_GAME,
        JOIN_GAME,
		DISCON
    };

    virtual const Code GetCode() const = 0;
};

struct AuthorizationClientCommand : public ClientCommand {
    string login;
    string password;

    virtual const Code GetCode() const override { return AUTH_REQ; }

    AuthorizationClientCommand(string login, string password) : login(login),
                                                                password(password) { }
};

struct RegistrationClientCommand : public ClientCommand {
    string login;
    string password;

    virtual const Code GetCode() const override { return REG_REQ; }

    RegistrationClientCommand(string login, string password) : login(login),
                                                               password(password) { }
};

struct GameListClientRequest : public ClientCommand {
    virtual const Code GetCode() const override { return SERVER_LIST_REQ; }
};

struct CreateGameClientCommand : public ClientCommand {
    string title;

    virtual const Code GetCode() const override { return CREATE_GAME; }

    CreateGameClientCommand(string title) : title(title) { }
};

struct JoinGameClientCommand : public ClientCommand {
    int id;

    virtual const Code GetCode() const override { return JOIN_GAME; }

    JoinGameClientCommand(int id) : id(id) { }
};

struct ServerCommand {
    enum Code {
        AUTH_SUCCESS = 1,
        REG_SUCCESS,
        AUTH_ERROR,
        REG_ERROR,
        GAME_CREATE_SUCCESS,
        GAME_CREATE_ERROR,
        GAME_LIST,
        GAME_JOIN_SUCCESS,
        GAME_JOIN_ERROR,
        CONNECTION_ERROR,
        COMMAND_CODE_ERROR
    };

    virtual const Code GetCode() const = 0;
};

struct AuthSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return AUTH_SUCCESS; }
};

struct RegSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return REG_SUCCESS; }
};

struct AuthErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return AUTH_ERROR; }
};

struct RegErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return REG_ERROR; }
};

struct GameCreateSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return GAME_CREATE_SUCCESS; }
};

struct GameCreateErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return GAME_CREATE_ERROR; }
};

struct GameJoinSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return GAME_JOIN_SUCCESS; }
};

struct GameJoinErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return GAME_JOIN_ERROR; }
};

struct GameListServerCommand : public ServerCommand {
    const std::list<uptr<Game>> * const games;
    GameListServerCommand(const std::list<uptr<Game>> * const games) : games(games) { }

    virtual const Code GetCode() const override { return GAME_LIST; }
};

struct ConnectionErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return CONNECTION_ERROR; }
};

struct CommandCodeErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return COMMAND_CODE_ERROR; }
};
