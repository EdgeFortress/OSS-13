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

	static sf::TcpSocket socket;

public:
	static uptr<std::thread> thread;

	static ThreadSafeQueue<ClientCommand *> commandQueue;
	static ThreadSafeQueue<ServerCommand::Code> answerQueue;

	static Result Connect(const string ip, const int port);
	static void SendCommand();
};