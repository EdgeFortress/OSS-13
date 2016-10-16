#pragma once

#include <SFML/Network.hpp>

#include <memory>
#include <mutex>
#include <queue>
#include <string>

using std::string;

template<class T>
using uptr = std::unique_ptr<T>;

template<class T>
class ThreadSafeQueue {
private:
	std::queue<T> queue;
	std::mutex mutex;
public:
	void Push(T t) {
		mutex.lock();
		queue.push(t);
		mutex.unlock();
	}

	T Front() {
		mutex.lock();
		T t = queue.front();
		mutex.unlock();
		return t;
	}

	void Pop() {
		mutex.lock();
		queue.pop();
		mutex.unlock();
	}

	bool Empty() {
		mutex.lock();
		bool empty = queue.empty();
		mutex.unlock();
		return empty;
	}
};

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

/*enum Command_code {
	AUTH_CODE = 0,
	REG_CODE = 1
};*/

//enum Result {
//	OK = 0, 
//	AUTH_ERROR = 1, 
//	REG_ERROR = 2,
//	CONNECTION_ERROR = 3, 
//	COMMAND_CODE_ERROR = 4
//};

const int PORT = 55678;