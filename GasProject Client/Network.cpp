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

void Network::SendCommand() {
	while (!commandQueue.Empty()) {
		sf::Packet pac;
		switch (commandQueue.Front()->GetCode()) {
		case ClientCommand::AUTH_REQ:
			pac << commandQueue.Front()->GetCode() << commandQueue.Front()->login << commandQueue.Front()->password;
		case ClientCommand::REG_REQ:
			pac << commandQueue.Front()->GetCode() << commandQueue.Front()->login << commandQueue.Front()->password;
		}
		socket.send(pac);
		commandQueue.Pop();

		sleep(seconds(0.01f));
		if (socket.receive(pac)) {
			answerQueue.Push(ServerCommand::CONNECTION_ERROR);
			break;
		}
		int res;
		pac >> res;
		answerQueue.Push((ServerCommand::Code)res);
	}
}

string Network::ip;
int Network::port;
uptr<std::thread> Network::thread;
sf::TcpSocket Network::socket;
ThreadSafeQueue<ClientCommand *> Network::commandQueue;
ThreadSafeQueue<ServerCommand::Code> Network::answerQueue;