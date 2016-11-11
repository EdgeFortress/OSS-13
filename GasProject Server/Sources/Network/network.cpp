#include <iostream>

#include "network.hpp"
#include "Server.hpp"
#include "player.hpp"
#include "World/World.hpp"
#include "Database/UsersDB.hpp"

#include "Common/NetworkConst.hpp"

using namespace std;
using namespace sf;

void ListeningSocket::listening() {
    sf::TcpListener listener;
    listener.setBlocking(false);
    listener.listen(Global::PORT);
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
                packet << *(game.get());
            }
            break;
        }
        case ServerCommand::Code::GRAPHICS_FULL_UPDATE: {
            GraphicsFullUpdateServerCommand *command = dynamic_cast<GraphicsFullUpdateServerCommand *>(serverCommand);
            packet << *(command->camera);
        }
    }

    return packet;
}

Packet &operator<<(Packet &packet, Game &game) {
    packet << sf::Int32(game.id);
    packet << sf::String(game.title);
    packet << sf::Int32(game.players.size());
    return packet;
}

void Camera::PackDifferences(Packet &packet) {
    for (auto &vect : visibleBlocks)
        for (auto &block : vect) {
            if (block->GetDifferences().size()) {
                packet << sf::Int32(block->ID());
                packet << sf::Int32(block->GetDifferences().size());
                Server::log << "Send diffences of block" << block->ID() << "(" << block->GetDifferences().size() << ")" << endl;
                for (const Diff &diff : block->GetDifferences())
                    packet << diff;
            }
        }
}

Packet &operator<<(Packet &packet, const Diff &diff) {
    packet << Int32(diff.GetType());
    packet << Int32(diff.x) << Int32(diff.y);
    packet << Int32(diff.objectNum);
    switch (diff.GetType()) {
        case Diff::Type::MOVE: {
            const MoveDiff &moveDiff = dynamic_cast<const MoveDiff &>(diff);
            packet << Int32(moveDiff.toX) << Int32(moveDiff.toY);
            break;
        }
        case Diff::Type::ADD: {
            const AddDiff &addDiff = dynamic_cast<const AddDiff &>(diff);
            break;
        }
        case Diff::Type::REMOVE: {
            const RemoveDiff &removeDiff = dynamic_cast<const RemoveDiff &>(diff);
            break;
        }
    }
    return packet;
}

Packet &operator<<(Packet &packet, const Camera &camera) {
    sf::Int32 cameraCentreX, cameraCentreY;
    cameraCentreX = camera.GetPosition()->X() - camera.visibleBlocks[0][0]->GetTile(0, 0)->X();
    cameraCentreY = camera.GetPosition()->Y() - camera.visibleBlocks[0][0]->GetTile(0, 0)->Y();
    packet << cameraCentreX << cameraCentreY;
    for (auto &vect : camera.visibleBlocks)
        for (const Block *block : vect)
            packet << *block;
    return packet;
}

Packet &operator<<(Packet &packet, const Block &block) {
    packet << sf::Int32(block.ID());
    for (auto &vect : block.tiles)
        for (const Tile *tile : vect)
            packet << *tile;
    return packet;
}

Packet &operator<<(Packet &packet, const Tile &tile) {
    packet << sf::Int32(tile.content.size());
    for (auto &obj : tile.content)
        packet << *obj;
    return packet;
}

Packet &operator<<(Packet &packet, const Object &obj) {
    packet << sf::Int32(obj.sprite);
    return packet;
}

bool ListeningSocket::active = false;
Server *ListeningSocket::server;
int ListeningSocket::port;
uptr<std::thread> ListeningSocket::listeningThread;