#pragma once

#include <memory>
#include <mutex>
#include <stack>
#include <string>

using std::string;

template<class T>
using uptr = std::unique_ptr<T>;

template<class T>
class ThreadSafeStack {
private:
	std::stack<T> stack;
	std::mutex mutex;
public:
	void Push(T &t) {
		mutex.lock();
		stack.push(t);
		mutex.unlock();
	}

	T &Pop() {
		mutex.lock();
		T &t = stack.top();
		mutex.unlock();
		return t;
	}
};

struct ClientCommand {
	enum Code {
		AUTH_REQ = 0,
		REG_REQ = 1
	};
	virtual const Code GetCode() const = 0;
};

struct AuthorizationClientCommand : public ClientCommand {
	string login;
	string password;
	virtual const Code GetCode() const override { return AUTH_REQ; }
};

struct RegistrationClientCommand : public ClientCommand {
	string login;
	string password;
	virtual const Code GetCode() const override { return REG_REQ; }
};

struct ServerCommand {
	enum Code {
		COMMAND_SUCCESS = 0
	};
	virtual const Code GetCode() const = 0;
};

struct SuccessServerCommand : public ServerCommand {
	virtual const Code GetCode() const override { return COMMAND_SUCCESS; }
};

enum Command_code {
	AUTH_CODE = 0,
	REG_CODE = 1
};

enum Result {
	OK = 0, 
	AUTH_ERROR = 1, 
	REG_ERROR = 2,
	CONNECTION_ERROR = 3, 
	COMMAND_CODE_ERROR = 4
};

const int PORT = 55678;