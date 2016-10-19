#include <iostream>

#include "network.hpp"
#include "Server.hpp"
#include "player.hpp"
#include "users_database.hpp"

#include <net_const.hpp>

using namespace std;
using namespace sf;

void ListeningSocket::listening() {
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

void ListeningSocket::Start(Server *server) {
    if (active) return;
    ListeningSocket::server = server;
    active = true;
    listeningThread.reset(new std::thread(ListeningSocket::listening));
}

void ListeningSocket::Stop() {
    if (!active) return;
    active = false;
    listeningThread->join();
}

Connection::Connection(sf::TcpSocket *socket, Server *server, Player *player) : socket(socket),
                                                                                server(server),
                                                                                player(player),
                                                                                thread(new std::thread(session, this)) {

}

void Connection::session(Connection *inst) {
    cout << "New client is connected" << endl;
    inst->socket->setBlocking(false);
    sf::Packet packet;
    bool isWorking;
    while (inst->active) {
        packet.clear();
        isWorking = false;
        if (!(inst->socket->receive(packet) == sf::Socket::NotReady)) {
            inst->parse(packet);
            isWorking = true;
        }
        while (!inst->player->commandQueue.Empty()) {
            packet.clear();
            ServerCommand *temp = inst->player->commandQueue.Pop();
            packet << temp;
            if (temp) delete temp;
            while (inst->socket->send(packet) == sf::Socket::Partial);
            isWorking = true;
        }
        if (!isWorking) sleep(seconds(0.01f));
    }
}

void Connection::parse(sf::Packet &pac) {
    sf::Int32 code;
    pac >> code;

    switch (code) {
        case ClientCommand::AUTH_REQ: {
            sf::String login, password;
            pac >> login >> password;
            if (server->Authorization(string(login), string(password))) {
                player->ckey = string(login);
                player->commandQueue.Push(new AuthSuccessServerCommand());
            }
            else
                player->commandQueue.Push(new AuthErrorServerCommand());
            break;
        }
        case ClientCommand::REG_REQ: {
            sf::String login, password;
            pac >> login >> password;
            if (server->Registration(string(login), string(password)))
                player->commandQueue.Push(new RegSuccessServerCommand());
            else
                player->commandQueue.Push(new RegErrorServerCommand());
            break;
        }
        default:
            player->commandQueue.Push(new CommandCodeErrorServerCommand());
    }
}

void Connection::Stop() {
    active = false;
    thread->join();
}

Packet &operator<<(Packet &packet, ServerCommand *serverCommand) {
	packet << sf::Int32(serverCommand->GetCode());
	return packet;
}

bool ListeningSocket::active = false;
Server *ListeningSocket::server;
int ListeningSocket::port;
uptr<std::thread> ListeningSocket::listeningThread;