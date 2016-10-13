#pragma once
#include <SFML\Network.hpp>
#include <list>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <map>
#include <sstream>
#include "net_const.hpp"

using namespace std;
using namespace sf;


class UsersDB
{
	string adr;
	map <string, string> all;
public:
	UsersDB(string adr);
	bool content(string &login, string &pass);
	bool add(string login, string pass);
};

class Netclient {
public:
	bool logedin = false;
	sf::TcpSocket* socket;
	Netclient(sf::TcpSocket* soc);
	Result login(string &s);
	Result signin(string &s);
	Result parse(sf::Packet & pac);
};

class Network {
public:

	static int port;
	static list<thread *> threads;
	static thread * main_net;
	static UsersDB UBD;

	Network(int port = ::PORT);
	static void listen();
	static void session(sf::TcpSocket *);
	~Network();
};