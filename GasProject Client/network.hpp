#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include "../GasProject Server/net_const.hpp"
using namespace std;

class Network {
	static string ip;
	static int port;
	static sf::TcpSocket socket;
public:
	Network(string ip = "127.0.0.1", int port = PORT);
	static Result connect();
	static Result send_command(Comand_code cc, list<string> args);
};