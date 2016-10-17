#pragma once

#include <string>
#include <list>

#include <useful.hpp>
#include <command.hpp>

class std::thread;
namespace sf {
    class TcpSocket;
    class Packet;
}
class ClientController;

using std::string;

class Network {
	static string ip;
	static int port;
    static bool connected;
    static ClientController *clientController;

	static sf::TcpSocket socket;

    static void session();
    static void sendCommands();
    static void parsePacket(sf::Packet &);

public:
    static bool needReceive;

	static uptr<std::thread> thread;

	static ThreadSafeQueue<ClientCommand *> commandQueue;
	//static ThreadSafeQueue<ServerCommand::Code> answerQueue;

	static bool Connect(const string ip, const int port, ClientController *);
};

sf::Packet &operator<<(sf::Packet &, ClientCommand *);



