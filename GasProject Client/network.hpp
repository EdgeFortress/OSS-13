#pragma once

#include <string>
#include <thread>
#include <list>

#include <SFML/Network.hpp>

#include "../GasProject Server/net_const.hpp"

using std::string;

class Network {
	static string ip;
	static int port;
	static uptr<std::thread> thread;
	static sf::TcpSocket socket;

	static ThreadSafeStack<ClientCommand> commandStack;

public:
	static Result Connect(const string ip, const int port);
	static Result SendCommand(Command_code cc, std::list<string> args);
};