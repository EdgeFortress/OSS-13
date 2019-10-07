#include "Connection.h"

#include <thread>
#include <string>
#include <memory>

#include <SFML/Network.hpp>

#include <Client.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/TileGrid/TileGrid.hpp>
#include <Graphics/TileGrid/Object.hpp>
#include <Graphics/UI/UI.hpp>
#include <Graphics/UI/UIModule/AuthUI.hpp>
#include <Graphics/UI/UIModule/GameProcessUI.hpp>

#include <Shared/ErrorHandling.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>
#include <Shared/Network/Protocol/ServerToClient/WindowData.h>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

#include "SyncCommandsProcessor.h"

using namespace std::string_literals;
using namespace network::protocol;

bool Connection::Start() {
	status = Status::WAITING;

	Connection::thread = std::make_unique<std::thread>(&session);
	Connection::syncCommandsProcessor = std::make_unique<network::SyncCommandsProcessor>();

	while (GetStatus() == Status::WAITING) {
		sf::sleep(sf::seconds(0.01f));
	}
	if (GetStatus() == Status::CONNECTED)
		return true;
	if (GetStatus() == Status::NOT_CONNECTED)
		return false;
	return false;
}

void Connection::Stop() {
	Connection::commandQueue.Push(new client::DisconnectionCommand());
	status = Status::NOT_CONNECTED;
	thread->join();
}

void Connection::ProcessSyncCommands() {
	syncCommandsProcessor->ProcessCommands();
}

Connection::Status Connection::GetStatus() { return status; }

void Connection::session() {
	sf::IpAddress ip = "localhost";
	sf::Socket::Status socketStatus = sf::Socket::Disconnected;

	const int maxAttempts = 2;
	for (int fails = 0; fails < maxAttempts; fails++) {
		for (auto port = Global::PORT_FIRST; port <= Global::PORT_LAST; port++) {
			socketStatus = socket.connect(ip, port, sf::seconds(1));
			if (socketStatus == sf::Socket::Done) {
				LOGI << "Connected to server with address " << ip << ":" << port;
				status = Status::CONNECTED;
				break;
			}
		}
		if (socketStatus == sf::Socket::Status::Done) {
			break;
		} else {
			LOGE << "Failed to find listening server port! (ip: " << ip << ")."
				 << (fails != maxAttempts - 1 ? " Check all ports again..." : "");
		}
	}

	if (socketStatus == sf::Socket::Done)
		status = Status::CONNECTED;
	else {
		status = Status::NOT_CONNECTED;
		return;
	}

	socket.setBlocking(false);

	while (status == Status::CONNECTED) {
		bool working = false;
		if (!commandQueue.Empty()) {
			sendCommands();
			working = true;
		}
		sf::Packet packet;
		if (socket.receive(packet) == sf::Socket::Done) {
			try {
				parsePacket(packet);
			} catch (const std::exception &e) {
				MANAGE_EXCEPTION(e);
			}
			working = true;
		}
		if (!working) sf::sleep(sf::seconds(0.01f));
	}
	if (!commandQueue.Empty())
		sendCommands();
}

void Connection::sendCommands() {
	while (!commandQueue.Empty()) {
		sf::Packet packet;
		uf::InputArchive ar(packet);
		Command *temp = commandQueue.Pop();
		ar << *temp;
		if (temp) delete temp;
		while (socket.send(packet) == sf::Socket::Partial);
	}
}

bool Connection::parsePacket(sf::Packet &packet) {
	uf::OutputArchive ar(packet);
	auto serializable = ar.UnpackSerializable();
	auto generalCommand = uptr<network::protocol::Command>(dynamic_cast<network::protocol::Command *>(serializable.release()));

	if (!generalCommand) {
		LOGE << "Unknown serializable (id is 0x" << std::hex << serializable->SerID() << ") is received!";
		return false;
	}

	if (dynamic_cast<server::GameJoinSuccessCommand *>(generalCommand.get())) {
		return true;
	}
	if (dynamic_cast<server::GameJoinErrorCommand *>(generalCommand.get())) {
		return true;
	}

	syncCommandsProcessor->AddCommand(std::move(generalCommand));
	return true;
}

Connection::Status Connection::status = Connection::Status::INACTIVE;
uptr<std::thread> Connection::thread;
sf::TcpSocket Connection::socket;
uf::ThreadSafeQueue<Command *> Connection::commandQueue;
uptr<network::CommandsProcessor> Connection::syncCommandsProcessor;
