#include <iostream>

#include "network.hpp"
#include "Server.hpp"
#include "player.hpp"
#include "users_database.hpp"

#include <net_const.hpp>

using namespace std;
using namespace sf;

Player::Player(Server *server, sf::TcpSocket *raw_pointer_socket) : server(server),
                                                                    socket(raw_pointer_socket),
                                                                    thread(new std::thread(Network::clientSession, socket.get(), this)) {
}

bool Player::authorization(string &login, string &password) {
	if (server->UDB->Check(login, password)) {
		cout << "Player is authorized: " << login << ' ' << password << endl;
		return true;
	}
	cout << "Wrong login data received: " << login << ' ' << password << endl;
	return false;
}

bool Player::registration(string &login, string &password) {
	if (server->UDB->Add(login, password)) { 
		cout << "New player is registrated: " << login << ' ' << password << endl;
		return true;
	}
	return false;
}

void Player::parse(sf::Packet &pac) {
	sf::Int32 code;
	pac >> code;
	
    switch (code) {
        case ClientCommand::AUTH_REQ: {
            sf::String login, password;
            pac >> login >> password;
            if (logedin = authorization(string(login), string(password)))
                commandQueue.Push(new AuthSuccessServerCommand());
            else
                commandQueue.Push(new AuthErrorServerCommand());
            break;
        }
        case ClientCommand::REG_REQ: {
            sf::String login, password;
            pac >> login >> password;
            if (registration(string(login), string(password)))
                commandQueue.Push(new RegSuccessServerCommand());
            else
                commandQueue.Push(new RegErrorServerCommand());
            break;
        }
        default:
            commandQueue.Push(new CommandCodeErrorServerCommand());
    }
}

void Network::ListeningSocket::listening() {
    sf::TcpListener listener;
    listener.listen(PORT);
    sf::TcpSocket *socket = new sf::TcpSocket;
    while (active) {
        if (listener.accept(*socket) == sf::TcpSocket::Done) {
            server->AddPlayer(new Player(server, socket));
            socket = new sf::TcpSocket;
        } else {
            cout << "New connection accepting error" << endl;
            active = false;
            break;
        }
    }
    delete socket;
}

void Network::ListeningSocket::Start(Server *server) {
    if (active) return;
    ListeningSocket::server = server;
    active = true;
    listeningThread.reset(new std::thread(Network::ListeningSocket::listening));
}

void Network::ListeningSocket::Stop() {
    if (!active) return;
    active = false;
    listeningThread->join();
}

void Network::clientSession(sf::TcpSocket *socket, Player *player) {
	cout << "New client is connected" << endl;
    socket->setBlocking(false);
	sf::Packet packet;
    bool isWorking;
	while (true) {
        packet.clear();
        isWorking = false;
        if (!(socket->receive(packet) == sf::Socket::NotReady)) {
            player->parse(packet);
            isWorking = true;
        }
		while (!player->commandQueue.Empty()) {
            packet.clear();
			ServerCommand *temp = player->commandQueue.Pop();
			packet << temp;
			if (temp) delete temp;
			while (socket->send(packet) == sf::Socket::Partial);
            isWorking = true;
		}
        if (!isWorking) sleep(seconds(0.01f));
	}
}

Packet &operator<<(Packet &packet, ServerCommand *serverCommand) {
	packet << sf::Int32(serverCommand->GetCode());
	return packet;
}

bool Network::ListeningSocket::active = false;
Server *Network::ListeningSocket::server;
int Network::ListeningSocket::port;
uptr<std::thread> Network::ListeningSocket::listeningThread;