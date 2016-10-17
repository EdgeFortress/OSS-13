#pragma once

#include <string>

using std::string;

struct ClientCommand {
    enum Code {
        AUTH_REQ = 1,
        REG_REQ = 2
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

struct ServerCommand {
    enum Code {
        AUTH_SUCCESS = 1,
        REG_SUCCESS,
        AUTH_ERROR,
        REG_ERROR,
        CONNECTION_ERROR,
        COMMAND_CODE_ERROR
    };

    virtual const Code GetCode() const = 0;
};

struct AuthSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return AUTH_SUCCESS; }
};

struct RegSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return AUTH_SUCCESS; }
};

struct AuthErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return AUTH_ERROR; }
};

struct RegErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return REG_ERROR; }
};

struct ConnectionErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return CONNECTION_ERROR; }
};

struct CommandCodeErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return COMMAND_CODE_ERROR; }
};
