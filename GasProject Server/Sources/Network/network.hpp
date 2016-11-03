#pragma once

#include <thread>

#include <SFML/Network.hpp>

#include "Common/Useful.hpp"
#include "Common/Ñommand.hpp"

class Player;
class Server;

class ListeningSocket {
private:
    static bool active;
    static Server *server;
    static int port;
    static uptr<std::thread> listeningThread;

    static void listening();
    
    ListeningSocket() = default;
    ListeningSocket(ListeningSocket &) = delete;
    ~ListeningSocket() = default;

public:
    static void Start(Server *);
    static void Stop();
};

class Connection {
private:
    bool active = true;
    Server *server;
    Player *player;
    uptr<sf::TcpSocket> socket;
    uptr<std::thread> thread;

    static void session(Connection *connection);
    void parse(sf::Packet &pac);

public:
    Connection(sf::TcpSocket *, Server *, Player *player);
    void Stop();
};

sf::Packet &operator<<(sf::Packet &, ServerCommand *);