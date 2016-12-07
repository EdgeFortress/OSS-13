#include <SFML/Network.hpp>

#include "NetworkController.hpp"
#include "Server.hpp"
#include "Player.hpp"
#include "Common/NetworkConst.hpp"
#include "Common/Ñommand.hpp"

void NetworkController::working() {
    sf::TcpListener listener;
    listener.listen(Global::PORT);

    sf::SocketSelector selector;

    selector.add(listener);

    std::list< std::pair<wptr<Player>, uptr<sf::TcpSocket>> > sockets;

    while (active) {
        if (selector.wait(TIMEOUT)) {
            // Receiving from client
            if (selector.isReady(listener)) {
                sf::TcpSocket *socket = new sf::TcpSocket;
                if (listener.accept(*socket) == sf::TcpSocket::Done) {
                    sptr<Player> player = std::make_shared<Player>();
                    Server::Get()->AddPlayer(player);
                    selector.add(*socket);
                    sockets.push_back(std::pair<wptr<Player>, uptr<sf::TcpSocket>>(player, uptr<sf::TcpSocket>(socket)));
                } else
                    Server::log << "New connection accepting error" << endl;
            }
            else {
                for (auto iter = sockets.begin(); iter != sockets.end();) {
                    if (selector.isReady(*iter->second)) {
                        sf::TcpSocket *socket = iter->second.get();
                        sptr<Player> player = iter->first.lock();

                        if (!player || !(player->IsConnected())) {
                            selector.remove(*socket);
                            iter = sockets.erase(iter);
                            continue;
                        } 

                        sf::Packet packet;
                        sf::Socket::Status status = socket->receive(packet);
                        switch (status) {
                            case sf::Socket::Done:
                                parsePacket(packet, player.get());
                                break;
                            case sf::Socket::Disconnected:
                                Server::log << "Lost client" << player->GetCKey() << "signal" << endl;
                                player->connected = false;
                                selector.remove(*socket);
                                iter = sockets.erase(iter);
                                continue;
                                break;
                        }

                    }

                    iter++;
                }
            }
        }

        // Sending to client
        for (auto &pair : sockets) {
            sptr<Player> player = pair.first.lock();
            while (!player->commandsToClient.Empty()) {
                sf::Packet packet;
                ServerCommand *temp = player->commandsToClient.Pop();
                packet << temp;
                if (temp) delete temp;
                pair.second->send(packet);
            }
        }
    }
}

void NetworkController::parsePacket(sf::Packet &packet, Player *player) {
    sf::Int32 code;
    packet >> code;

    switch (ClientCommand::Code(code)) {
        case ClientCommand::Code::AUTH_REQ: {
            sf::String login, password;
            packet >> login >> password;
            player->Authorize(login, password);
            break;
        }
        case ClientCommand::Code::REG_REQ: {
            sf::String login, password;
            packet >> login >> password;
            player->Register(login, password);
            break;
        }
        case ClientCommand::Code::CREATE_GAME: {
            sf::String title;
            packet >> title;
            Server::log << "Request for creating game" << endl;
            break;
        }
        case ClientCommand::Code::SERVER_LIST_REQ: {
            player->UpdateServerList();
            break;
        }
        case ClientCommand::Code::JOIN_GAME: {
            sf::Int32 id;
            packet >> id;
            player->JoinToGame(id);
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
        case ClientCommand::Code::WEST: {
            auto mob = player->GetMob();
            if (mob)
                mob->MoveWest();
            break;
        }
        case ClientCommand::Code::SEND_CHAT_MESSAGE: {
            std::vector<std::wstring> message;
            sf::Int32 size;
            std::wstring str;
            packet >> size;
            for (int i = 0; i < int(size); i++)
                packet >> str, message.push_back(str);
            player->game->SendChatMessage(message, player);
            player->game->GetChat()->AddMessage(message, player);
            break;
        }
        case ClientCommand::Code::DISCONNECT: {
            Server::log << "Client" << player->GetCKey() << "disconnected" << endl;
            player->connected = false;
            break;
        }
        default:
            Server::log << "Unknown Command received from" << player->GetCKey() << endl;
    }
}

NetworkController::NetworkController() {
    active = false;
}

void NetworkController::Start() {
    if (active) return;
    active = true;
    thread.reset(new std::thread(&NetworkController::working, this));
}

void NetworkController::Stop() {
    if (!active) return;
    active = false;
    thread->join();
}