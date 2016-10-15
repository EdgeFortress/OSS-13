#pragma once

#include <list>
#include <iostream>
#include <fstream>
#include <thread>
#include <map>
#include <sstream>

#include <SFML\Network.hpp>

#include "net_const.hpp"

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

class Netclient {
private:
	bool logedin = false;
	uptr<sf::TcpSocket> &socket;

public:
	Netclient(uptr<sf::TcpSocket> &socket);
	bool Authorization(string &login, string &password);
	bool Registration(string &login, string &password);
	void Parse(sf::Packet & pac);
};

class Network {
private:
	static bool inProcess;
	static int port;
	static uptr<thread> listeningThread;

	static list<thread *> threads;

	static void clientSession(sf::TcpSocket *client);
	static void listen();

public:
	static UsersDB UDB;
	static ThreadSafeQueue<ServerCommand *> commandQueue;

	static void Initialize(const int port);
	static void WIP_Wait();
};