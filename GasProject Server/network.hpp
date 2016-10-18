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

namespace Network {
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

    static void clientSession(sf::TcpSocket *, Player *);
}

Packet &operator<<(Packet &, ServerCommand *);