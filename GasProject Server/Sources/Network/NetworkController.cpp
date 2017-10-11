#include <SFML/Network.hpp>

#include "NetworkController.hpp"
#include "Shared/Global.hpp"
#include "Connection.hpp"
#include "Server.hpp"
#include "Player.hpp"

void NetworkController::working() {
    sf::TcpListener listener;
    listener.listen(Global::PORT);

    sf::SocketSelector selector;

    selector.add(listener);

    while (active) {
        if (selector.wait(TIMEOUT)) {
            // Receiving from client
            if (selector.isReady(listener)) {
                sf::TcpSocket *socket = new sf::TcpSocket;
                if (listener.accept(*socket) == sf::TcpSocket::Done) {
                    selector.add(*socket);
					Connection *new_connection = new Connection();
					new_connection->socket.reset(socket);
					connections.push_back(sptr<Connection>(new_connection));
                } else
                    Server::log << "New connection accepting error" << std::endl;
            } else {
                for (auto iter = connections.begin(); iter != connections.end();) {
					
                    if (selector.isReady(*iter->get()->socket)) {
                        sf::TcpSocket *socket = iter->get()->socket.get();
                        sptr<Player> player = (*iter)->player;

                        sf::Packet packet;
                        sf::Socket::Status status = socket->receive(packet);
                        switch (status) {
                            case sf::Socket::Done:
								if (!parsePacket(packet, *iter)) {
									selector.remove(*socket);
									iter = connections.erase(iter);
									continue;
								}
                                break;
                            case sf::Socket::Disconnected:
								if (player) {
									Server::log << "Lost client" << player->GetCKey() << "signal" << std::endl;
								} else
									Server::log << "Lost unregistered client signal" << std::endl;
                                selector.remove(*socket);
                                iter = connections.erase(iter);
                                continue;
                                break;
                        }

                    }

                    iter++;
                }
            }
        }

        // Sending to client
        for (auto &connection : connections) {
            while (!connection->commandsToClient.Empty()) {
				sf::Packet packet;
                ServerCommand *temp = connection->commandsToClient.Pop();
                packet << temp;
                if (temp) delete temp;
				connection->socket->send(packet);
            }
        }
    }
}

bool NetworkController::parsePacket(sf::Packet &packet, sptr<Connection> &connection) {
    sf::Int32 code;
    packet >> code;

    switch (ClientCommand::Code(code)) {
        case ClientCommand::Code::AUTH_REQ: {
            sf::String login, password;
            packet >> login >> password;
			
			bool secondConnection = false;
			for (auto &connection : connections) {
				if (connection->player && connection->player->GetCKey() == login) {
					secondConnection = true;
					Server::log << "Player" << login.toAnsiString() << password.toAnsiString() << "is trying to authorize second time" << std::endl;
					break;
				}
			}

			if (!secondConnection) {
				if (Player *player = Server::Get()->Authorization(string(login.toAnsiString()), string(password.toAnsiString()))) {
					player->SetConnection(connection);
					connection->player = sptr<Player>(player);
					connection->commandsToClient.Push(new AuthSuccessServerCommand());
					break;
				}
			}
			connection->commandsToClient.Push(new AuthErrorServerCommand());
            break;
        }
        case ClientCommand::Code::REG_REQ: {
            sf::String login, password;
            packet >> login >> password;
			if (Server::Get()->Registration(login.toAnsiString(), password.toAnsiString()))
				connection->commandsToClient.Push(new RegSuccessServerCommand());
			else
				connection->commandsToClient.Push(new RegErrorServerCommand());
            break;
        }
        case ClientCommand::Code::CREATE_GAME: {
            sf::String title;
            packet >> title;
            Server::log << "Request for creating game" << std::endl;
            break;
        }
        case ClientCommand::Code::SERVER_LIST_REQ: {
			
				connection->player->UpdateServerList();
            break;
        }
        case ClientCommand::Code::JOIN_GAME: {
            sf::Int32 id;
            packet >> id;
			if (connection->player) {
				if (Game *game = Server::Get()->JoinGame(id, connection->player)) {
					connection->commandsToClient.Push(new GameJoinSuccessServerCommand());
				} else {
					connection->commandsToClient.Push(new GameJoinErrorServerCommand());
				}
			}
            break;
        }
        case ClientCommand::Code::MOVE: {
            sf::Int8 direction;
            packet >> direction;
			if (connection->player)
				connection->player->Move(uf::Direction(direction));
            break;
        }
        case ClientCommand::Code::CLICK_OBJECT: {
            sf::Int32 id;
            packet >> id;
            if (connection->player)
                connection->player->ClickObject(id);
            break;
        }
		case ClientCommand::Code::SEND_CHAT_MESSAGE: {
			std::string message;
			packet >> message;
			if (connection->player)
				connection->player->ChatMessage(message);
			break;
		}
		case ClientCommand::Code::BUILD: {
			if (connection->player)
				connection->player->Build();
			break;
		}
		case ClientCommand::Code::GHOST: {
			if (connection->player)
				connection->player->Ghost();
			break;
		}
        case ClientCommand::Code::DISCONNECT: {
			if (connection->player)
				Server::log << "Client" << connection->player->GetCKey() << "disconnected" << std::endl;
			return false;
            break;
        }
        default:
			if (connection->player)
				Server::log << "Unknown Command received from" << connection->player->GetCKey() << std::endl;
			else
				Server::log << "Unknown Command received from unregistered client" << std::endl; 
    }
	return true;
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