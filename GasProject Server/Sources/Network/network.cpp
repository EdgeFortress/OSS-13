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
        while (!inst->player->commandsToClient.Empty()) {
            packet.clear();
            ServerCommand *temp = inst->player->commandsToClient.Pop();
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
                player->commandsToClient.Push(new AuthSuccessServerCommand());
                player->commandsToClient.Push(new GameListServerCommand());
            }
            else
                player->commandsToClient.Push(new AuthErrorServerCommand());
            break;
        }
        case ClientCommand::Code::REG_REQ: {
            sf::String login, password;
            pac >> login >> password;
            if (server->Registration(string(login), string(password)))
                player->commandsToClient.Push(new RegSuccessServerCommand());
            else
                player->commandsToClient.Push(new RegErrorServerCommand());
            break;
        }
        case ClientCommand::Code::CREATE_GAME: {
            sf::String title;
            pac >> title;
            if (server->CreateGame(title))
                player->commandsToClient.Push(new GameCreateSuccessServerCommand());
            else
                player->commandsToClient.Push(new GameCreateSuccessServerCommand());
            break;
        }
        case ClientCommand::Code::SERVER_LIST_REQ: {
            player->commandsToClient.Push(new GameListServerCommand());
            break;
        }
        case ClientCommand::Code::JOIN_GAME: {
            sf::Int32 id;
            pac >> id;
            server->JoinGame(id, player);
            break;
        }
        case ClientCommand::Code::NORTH: {
            auto mob = player->GetMob();
            if (mob)
                mob->MoveNorth();
            break;
        }
        case ClientCommand::Code::SOUTH: {
            auto mob = player->GetMob();
            if (mob)
                mob->MoveSouth();
            break;
        }
        case ClientCommand::Code::EAST: {
            auto mob = player->GetMob();
            if (mob)
                mob->MoveEast();
            break;
        }
        case ClientCommand::Code::SEND_CHAT_MESSAGE: {
            std::vector<std::wstring> message;
            sf::Int32 size;
            std::wstring str;
            pac >> size;
            for (int i = 0; i < int(size); i++)
                pac >> str, message.push_back(str);
            player->game->SendChatMessage(message, player);
            player->game->GetChat()->AddMessage(message, player);
            break;
        }
        case ClientCommand::Code::WEST: {
            auto mob = player->GetMob();
            if (mob)
                mob->MoveWest();
            break;
        }
        case ClientCommand::Code::DISCONNECT: {
            active = false;
            Server::log << "Client" << player->ckey << "disconnected" << endl;
            break;
        }
        default:
            Server::log << "Unknown Command received from" << player->ckey << endl;
            player->commandsToClient.Push(new CommandCodeErrorServerCommand());
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
        case ServerCommand::Code::GRAPHICS_UPDATE: {
            GraphicsUpdateServerCommand *command = dynamic_cast<GraphicsUpdateServerCommand *>(serverCommand);
            packet << sf::Int32(command->options);
            if (command->options & GraphicsUpdateServerCommand::Option::BLOCKS_SHIFT) {
                packet << sf::Int32(command->firstBlockX) << sf::Int32(command->firstBlockY);
                packet << sf::Int32(command->blocksInfo.size());
                for (auto &blockInfo : command->blocksInfo)
                    packet << blockInfo;
            }
            if (command->options & GraphicsUpdateServerCommand::Option::CAMERA_MOVE) {
                packet << sf::Int32(command->cameraX) << sf::Int32(command->cameraY);
            }
            if (command->options & GraphicsUpdateServerCommand::Option::DIFFERENCES) {
                packet << sf::Int32(command->diffs.size());
                for (auto &diff : command->diffs)
                    packet << *(diff);
            }
            break;
        }
        case ServerCommand::Code::SEND_CHAT_MESSAGE: {
            auto c = dynamic_cast<SendChatMessageServerCommand *>(serverCommand);
            packet << c->playerName;
            packet << sf::Int32(c->message.size());
            for (auto &str : c->message) {
                packet << str;
            }
            break;
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

Packet &operator<<(Packet &packet, const Diff &diff) {
    if (diff.GetType() == Diff::Type::NONE) return packet;
    packet << Int32(diff.GetType());
    packet << Int32(diff.block->X());
    packet << Int32(diff.block->Y());
    packet << Int32(diff.x) << Int32(diff.y);
    packet << Int32(diff.objectNum);
    switch (diff.GetType()) {
        case Diff::Type::MOVE: {
            const MoveDiff &moveDiff = dynamic_cast<const MoveDiff &>(diff);
            packet << Int32(moveDiff.toX) << Int32(moveDiff.toY) << Int32(moveDiff.toObjectNum);
            break;
        }
        case Diff::Type::ADD: {
            const AddDiff &addDiff = dynamic_cast<const AddDiff &>(diff);
            packet << Int32(addDiff.sprite);
            packet << String(addDiff.name);
            break;
        }
        case Diff::Type::REMOVE: {
            const RemoveDiff &removeDiff = dynamic_cast<const RemoveDiff &>(diff);
            break;
        }
    }
    return packet;
}

Packet &operator<<(Packet &packet, const BlockInfo &blockInfo) {
    packet << sf::Int32(blockInfo.x) << sf::Int32(blockInfo.y);
    for (auto &tileInfo : blockInfo.tiles)
        packet << tileInfo;
    return packet;
}

Packet &operator<<(Packet &packet, const TileInfo &tileInfo) {
    packet << sf::Int32(tileInfo.content.size()) << sf::Int32(tileInfo.sprite);
    for (auto &objInfo : tileInfo.content) {
        packet << objInfo;
    }
    return packet;
}

Packet &operator<<(Packet &packet, const ObjectInfo &objInfo) {
    packet << sf::Int32(objInfo.sprite) << sf::String(objInfo.name);
    return packet;
}

bool ListeningSocket::active = false;
Server *ListeningSocket::server;
int ListeningSocket::port;
uptr<std::thread> ListeningSocket::listeningThread;