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
	bool contain(string &login, string &pass);
	bool add(string login, string pass);
};

class Netclient {
private:
	bool logedin = false;
	uptr<sf::TcpSocket> &socket;

public:
	Netclient(uptr<sf::TcpSocket> &socket);
	Result Authorization(string &s);
	Result Registration(string &s);
	Result parse(sf::Packet & pac);
};

class Network {
private:
	static bool inProcess;
	static int port;
	static uptr<thread> listeningThread;

	static list<thread *> threads;
	static ThreadSafeStack<ServerCommand> commandStack;

	static void session(sf::TcpSocket *client);
	static void listen();

public:
	static UsersDB UDB;

	static void Initialize(const int port);
	static void WIP_Wait();
};