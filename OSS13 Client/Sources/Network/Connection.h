#pragma once

#include <string>
#include <thread>

#include <SFML/Network.hpp>

#include <Shared/ThreadSafeQueue.hpp>
#include <Shared/Network/Protocol/Command.h>
#include <Shared/Network/Protocol/CommandsProcessor.h>

class Connection {
public:
	enum class Status : char {
		INACTIVE = 0,
		WAITING,
		CONNECTED,
		NOT_CONNECTED,
	};

	static uf::ThreadSafeQueue<network::protocol::Command *> commandQueue;

	static bool Start();
	static void Stop();

	static void ProcessSyncCommands();

	static Status GetStatus();

private:
	static void session();
	static void sendCommands();
	static bool parsePacket(std::unique_ptr<sf::Packet> packet);

private:
	static Status status;

	static uptr<std::thread> thread;
	static sf::TcpSocket socket;

	static uptr<network::CommandsProcessor> syncCommandsProcessor;
};
