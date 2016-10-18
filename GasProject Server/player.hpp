#pragma once

#include <string>

#include <useful.hpp>

using std::string;

namespace sf {
    class TcpSocket;
    class Packet;
}

namespace std {
    thread;
}

class Player {
private:
    bool logedin = false;
    Server *server;
    uptr<sf::TcpSocket> socket;
    uptr<std::thread> thread;

    ThreadSafeQueue<ServerCommand *> commandQueue;

    void parse(sf::Packet & pac);
    bool authorization(string &login, string &password);
    bool registration(string &login, string &password);

public:
    Player(Server *server, sf::TcpSocket *socket);

    friend void Network::clientSession(sf::TcpSocket *socket, Player *player);
};