#include "NetworkController.hpp"

#include <SFML/Network.hpp>

#include <plog/Log.h>

#include <Shared/Global.hpp>
#include <Shared/Network/Archive.h>
#include <Shared/Network/Protocol/InputData.h>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>
#include <Shared/ErrorHandling.h>

#include <IServer.h>
#include <Player/Player.h>

#include "Connection.hpp"

using namespace network::protocol;

namespace {

std::unique_ptr<sf::TcpListener> createListener(uint16_t portFirst, uint16_t portLast) {
	auto listener = std::make_unique<sf::TcpListener>();

	sf::Socket::Status listenerStatus = sf::Socket::Status::Error;

	for (auto port = portFirst; port <= portLast; port++) {
		listenerStatus = listener->listen(port);

		if (listenerStatus == sf::Socket::Status::Done) {
			LOGI << "Listening port is " << port;
			break;
		} else {
			LOGI << "Failed to create listener at port " << port << "! SFML Socket status: " << listenerStatus;
		}
	}

	EXPECT_WITH_MSG(listenerStatus == sf::Socket::Status::Done, "Failed to create listening socket! No available ports.");

	return listener;
}

} // namespace

void NetworkController::working(std::unique_ptr<sf::TcpListener> listener) {
    sf::SocketSelector selector;

    selector.add(*listener);

    while (active) {
        if (selector.wait(sf::microseconds(TIMEOUT.count()))) {
            // Receiving from client
            if (selector.isReady(*listener)) {
                sf::TcpSocket *socket = new sf::TcpSocket;
                if (listener->accept(*socket) == sf::TcpSocket::Done) {
                    selector.add(*socket);
					Connection *new_connection = new Connection();
					new_connection->socket.reset(socket);
					connections.push_back(sptr<Connection>(new_connection));
                } else
                    LOGE << "New connection accepting error";
            } else {
                for (auto iter = connections.begin(); iter != connections.end();) {
					
                    if (selector.isReady(*iter->get()->socket)) {
                        sf::TcpSocket *socket = iter->get()->socket.get();
                        sptr<Player> player = (*iter)->player;

						auto packet = std::make_unique<sf::Packet>();
                        sf::Socket::Status status = socket->receive(*packet);
                        switch (status) {
                            case sf::Socket::Done:
								if (!parsePacket(std::move(packet), *iter)) {
									selector.remove(*socket);
									iter = connections.erase(iter);
									continue;
								}
                                break;
                            case sf::Socket::Disconnected:
								if (player) {
									LOGI << "Lost client " << player->GetCKey() << " connection";
								} else
									LOGI << "Lost unregistered client signal";
                                selector.remove(*socket);
                                iter = connections.erase(iter);
                                continue;
                        	default:
                        		EXPECT(false);
                        }

                    }

                    iter++;
                }
            }
        }

        // Sending to client
        for (auto &connection : connections) {
			while (!connection->commandsToClient.Empty()) {
				uf::Archive ar;
				network::protocol::Command *command = connection->commandsToClient.Pop();
				EXPECT(command);
				ar << *command;
				delete command;
				connection->socket->send(ar.GetPacket());
			}
        }
    }
}

bool NetworkController::parsePacket(std::unique_ptr<sf::Packet> packet, sptr<Connection> &connection) {
	uf::Archive ar(std::move(packet));
	auto serializable = ar.UnpackSerializable();
	auto generalCommand = uptr<network::protocol::Command>(dynamic_cast<network::protocol::Command *>(serializable.release()));

	if (!generalCommand) {
		LOGE << "Unknown serializable (id is 0x" << std::hex << serializable->SerID() << ") is received from "
			<< (connection->player ? connection->player->GetCKey() : "unknown client")
			<< "!";
		return false;
	}

	if (auto *command = dynamic_cast<client::AuthorizationCommand *>(generalCommand.get())) {
		bool secondConnection = false;
		for (auto &connection : connections) {
			if (connection->player && connection->player->GetCKey() == command->login) {
				secondConnection = true;
				LOGI << "Player " << command->login << " " << command->password << " is trying to authorize second time";
				break;
			}
		}

		if (!secondConnection) {
			if (Player *player = GServer->Authorization(command->login, command->password)) {
				player->SetConnection(connection);
				connection->player = sptr<Player>(player);
				connection->commandsToClient.Push(new network::protocol::server::AuthorizationSuccessCommand());
				return true;
			}
		}
		connection->commandsToClient.Push(new network::protocol::server::AuthorizationFailedCommand());
		return true;
	}

	if (auto *command = dynamic_cast<client::RegistrationCommand *>(generalCommand.get())) {
		if (GServer->Registration(command->login, command->password))
			connection->commandsToClient.Push(new network::protocol::server::RegistrationSuccessCommand());
		else
			connection->commandsToClient.Push(new network::protocol::server::RegistrationFailedCommand());
		return true;
	}

	if (dynamic_cast<client::JoinGameCommand *>(generalCommand.get())) {
		if (connection->player) {
			if (GServer->JoinGame(connection->player)) {
				connection->commandsToClient.Push(new network::protocol::server::GameJoinSuccessCommand());
			} else {
				connection->commandsToClient.Push(new network::protocol::server::GameJoinErrorCommand());
			}
		}
		return true;
	}

	if (dynamic_cast<client::DisconnectionCommand *>(generalCommand.get())) {
		if (connection->player) {
			LOGI << "Client " << connection->player->GetCKey() << " disconnected";
		}
		return false;
	}

	EXPECT(connection->player);
	connection->player->AddSyncCommandFromClient(std::forward<uptr<network::protocol::Command>>(generalCommand));
	return true;
}

NetworkController::NetworkController() {
    active = false;
}

void NetworkController::Start() {
    if (active) return;
    active = true;
	auto listener = createListener(Global::PORT_FIRST, Global::PORT_LAST);
	thread = std::make_unique<std::thread>(&NetworkController::working, this, std::move(listener));
}

void NetworkController::Stop() {
    if (!active) return;
    active = false;
    thread->join();
}