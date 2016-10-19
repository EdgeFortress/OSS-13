#pragma once

#include <string>

#include <useful.hpp>

using std::string;

namespace sf {
    class TcpSocket;
}

class Server;
class Connection;
struct ServerCommand;

class Player {
private:
    string ckey;
    Server *server;
    uptr<Connection> connection;

    ThreadSafeQueue<ServerCommand *> commandQueue;

public:
    Player(Server *server, sf::TcpSocket *socket);

    friend Connection;
};