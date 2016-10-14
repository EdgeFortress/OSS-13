#include <iostream>

#include "network.hpp"

using namespace std;
using namespace sf;

Result Network::Connect(string ip, int port) {
	Network::ip = ip;
	Network::port = port;
	if (socket.connect(ip, port, seconds(1)) != sf::Socket::Done) {
		return CONNECTION_ERROR;
	}
	else return OK;
}

Result Network::SendCommand(Comand_code cc, std::list<string> args) {
	sf::Packet pac;
	string s = string{ (char)cc };
	for (auto arg = args.begin(); arg != args.end(); arg++)
		s += " " + *arg;

	pac << s;
	socket.send(pac);
	int i = 0;
	//string s;
	while (i++ < 150) {
		sleep(seconds(0.01f));
		if (socket.receive(pac)) return CONNECTION_ERROR;
		pac >> s;
		return (Result)s[0];
	}
	return CONNECTION_ERROR;
}

string Network::ip;
int Network::port;
uptr<std::thread> thread;
sf::TcpSocket Network::socket;