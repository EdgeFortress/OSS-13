#pragma once

#include <list>
#include <iostream>
#include <fstream>
#include <thread>
#include <map>
#include <sstream>

#include <SFML\Network.hpp>

#include <useful.hpp>
#include <command.hpp>

class Player;
class Server;

using namespace std;
using namespace sf;

class UsersDB {
	string adr;
	map <string, string> all;
public:
	UsersDB(string adr);
	bool Contain(string &login, string &pass);
	bool Add(string login, string pass);
};

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