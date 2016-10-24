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
                cout << "New connection accepting error" << endl;
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
	case ClientCommand::CREATE_GAME: {
		sf::String title;
		pac >> title;
		if (server->CreateGame(title))
			player->commandQueue.Push(new GameCreateSuccessServerCommand());
		else
			player->commandQueue.Push(new GameCreateSuccessServerCommand());
		break;
	}
	case ClientCommand::SERVER_LIST_REQ: {
		player->commandQueue.Push(new GameListServerCommand(server->GetGamesList()));
		break;
	}
	case ClientCommand::JOIN_GAME: {
		sf::Int32 id;
		pac >> id;
		player->game = server->JoinGame(id, player);
		if (player->game)
			player->commandQueue.Push(new GameJoinSuccessServerCommand());
		else
			player->commandQueue.Push(new GameJoinErrorServerCommand());
		break;
	}
	case ClientCommand::DISCON: {
		active = false;
	}
        default:
            cout << "Unknown Command received from " << player->ckey << endl;
            player->commandQueue.Push(new CommandCodeErrorServerCommand());
    }
}

void Connection::Stop() {
    active = false;
    thread->join();
}

Packet &operator<<(Packet &packet, ServerCommand *serverCommand) {
    int code = serverCommand->GetCode();
	packet << sf::Int32(code);
    switch (code) {
        case ServerCommand::GAME_LIST: {
            GameListServerCommand *command = dynamic_cast<GameListServerCommand *>(serverCommand);
            packet << command->games->size();
            for (auto &game : *(command->games)) {
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