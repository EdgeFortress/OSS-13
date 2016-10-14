#include "network.hpp"
using namespace std;
using namespace sf;
string Network::ip;
int Network::port;
sf::TcpSocket Network::socket;

Network::Network(string ip, int port) {
	Network::ip = ip;
	Network::port = port;
}
Result Network::connect() {
	if (socket.connect(ip, port, seconds(20)) != sf::Socket::Done)
		return CONNECTION_ERROR;
	else return OK;
}

Result Network::send_command(Comand_code cc, list<string> args) {
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
		socket.receive(pac);
		if (pac.getDataSize() != 0) {
			pac >> s;
			return (Result)s[0];
		}
	}
	return CONNECTION_ERROR;
}