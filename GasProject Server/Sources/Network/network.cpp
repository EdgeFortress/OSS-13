#include <iostream>

#include "network.hpp"
#include "Server.hpp"
#include "player.hpp"
#include "Database/UsersDB.hpp"

#include "Common/NetworkConst.hpp"

using namespace std;
using namespace sf;

void ListeningSocket::listening() {
    sf::TcpListener listener;
    listener.setBlocking(false);
    listener.listen(PORT);
    sf::TcpSocket *socket = new sf::TcpSocket;
    while (active) {
        sf::TcpSocket::Status status = listener.accept(*socket);
        switch (status) {
            case sf::TcpSocket::Done: {
                server->AddPlayer(new Player(server, socket));
                socket = new sf::TcpSocket;
                break;
            }
            case sf::TcpSocket::NotReady: {
                sleep(seconds(0.01f));
                break;
            }
            default: {
				Server::log << "New connection accepting error" << endl;
                active = false;
                break;
            }
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
	Server::log << "New client is connected" << endl;
    inst->socket->setBlocking(false);
    sf::Packet packet;
    bool isWorking;
    while (inst->active) {
        packet.clear();
        isWorking = false;
		sf::Socket::Status status = inst->socket->receive(packet);
        if (status == sf::Socket::Done) {
            inst->parse(packet);
            isWorking = true;
        }
		if (status == sf::Socket::Disconnected) {
			inst->active = false;
			Server::log << "Lost client" << inst->player->ckey << "signal" << endl;
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

    switch (static_cast<ClientCommand::Code>(code)) {
        case ClientCommand::Code::AUTH_REQ: {
            sf::String login, password;
            pac >> login >> password;
            if (server->Authorization(string(login), string(password))) {
                player->ckey = string(login);
                player->commandQueue.Push(new AuthSuccessServerCommand());
                player->commandQueue.Push(new GameListServerCommand());
            }
            else
                player->commandQueue.Push(new AuthErrorServerCommand());
            break;
        }
        case ClientCommand::Code::REG_REQ: {
            sf::String login, password;
            pac >> login >> password;
            if (server->Registration(string(login), string(password)))
                player->commandQueue.Push(new RegSuccessServerCommand());
            else
                player->commandQueue.Push(new RegErrorServerCommand());
            break;
        }
        case ClientCommand::Code::CREATE_GAME: {
            sf::String title;
            pac >> title;
            if (server->CreateGame(title))
                player->commandQueue.Push(new GameCreateSuccessServerCommand());
            else
                player->commandQueue.Push(new GameCreateSuccessServerCommand());
            break;
        }
        case ClientCommand::Code::SERVER_LIST_REQ: {
            player->commandQueue.Push(new GameListServerCommand());
            break;
        }
        case ClientCommand::Code::JOIN_GAME: {
            sf::Int32 id;
            pac >> id;
            player->game = server->JoinGame(id, player);
            if (player->game)
                player->commandQueue.Push(new GameJoinSuccessServerCommand());
            else
                player->commandQueue.Push(new GameJoinErrorServerCommand());
            break;
        }
        case ClientCommand::Code::DISCONNECT: {
            active = false;
            Server::log << "Client" << player->ckey << "disconnected" << endl;
            break;
        }
        default:
            Server::log << "Unknown Command received from" << player->ckey << endl;
            player->commandQueue.Push(new CommandCodeErrorServerCommand());
    }
}

void Connection::Stop() {
    active = false;
	if (thread)
        thread->join();
}

Packet &operator<<(Packet &packet, ServerCommand *serverCommand) {
    ServerCommand::Code code = serverCommand->GetCode();
	packet << sf::Int32(code);
    switch (code) {
        case ServerCommand::Code::GAME_LIST: {
            packet << sf::Int32(Server::Get()->GetGamesList()->size());
            for (auto &game : *(Server::Get()->GetGamesList())) {
                packet << game.get();
            }
        }
    }

	return packet;
}

Packet &operator<<(Packet &packet, Game *game) {
    packet << sf::Int32(game->id);
    packet << sf::String(game->title);
    packet << sf::Int32(game->players.size());
    return packet;
}

bool ListeningSocket::active = false;
Server *ListeningSocket::server;
int ListeningSocket::port;
uptr<std::thread> ListeningSocket::listeningThread;