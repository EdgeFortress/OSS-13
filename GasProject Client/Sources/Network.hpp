#pragma once

#include <string>

#include <SFML/Network.hpp>

#include <Shared/ThreadSafeQueue.hpp>
#include <Shared/Network/Protocol/ClientCommand.h>

namespace std {
    class thread;
}

namespace sf {
    class TcpSocket;
    class Packet;
}

using std::string;

class Connection {
    enum class Status : char {
        INACTIVE = 0,
        WAITING,
        CONNECTED,
        NOT_CONNECTED,
    };
    static Status status;

    static sf::IpAddress serverIp;
    static int serverPort;
    static uptr<std::thread> thread;
    
    static sf::TcpSocket socket;

    static void session();
    static void sendCommands();
    static void parsePacket(sf::Packet &);

public:
    static uf::ThreadSafeQueue<network::protocol::ClientCommand *> commandQueue;

    static bool Start(const string ip, const int port);
    static void Stop();

    static Status GetStatus() { return status; }
};
