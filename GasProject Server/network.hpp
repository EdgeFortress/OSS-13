#pragma once

#include <list>
#include <thread>


#include <SFML\Network.hpp>

#include <useful.hpp>
#include <command.hpp>

class Player;
class Server;

using namespace std;
using namespace sf;

class ListeningSocket {
private:
    static bool active;
    static Server *server;
    static int port;
    static uptr<std::thread> listeningThread;

    static void listening();


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

Packet &operator<<(Packet &, ServerCommand *);