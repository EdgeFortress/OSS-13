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

bool Connection::Start(const std::string &ip, int port) {
	status = Status::WAITING;

	serverIp = ip;
	serverPort = port;
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
	if (socket.connect(serverIp, serverPort, sf::seconds(5)) != sf::Socket::Done)
		status = Status::NOT_CONNECTED;
	else
		status = Status::CONNECTED;

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
		LOGE << "Unknown serializable (id is 0x" << std::hex << serializable->Id() << ") is received!";
		return false;
	}

	if (auto *command = dynamic_cast<server::GameJoinSuccessCommand *>(generalCommand.get())) {
		return true;
	}
	if (auto *command = dynamic_cast<server::GameJoinErrorCommand *>(generalCommand.get())) {
		return true;
	}

	syncCommandsProcessor->AddCommand(std::move(generalCommand));
	return true;
}

sf::IpAddress Connection::serverIp;
int Connection::serverPort;
Connection::Status Connection::status = Connection::Status::INACTIVE;
uptr<std::thread> Connection::thread;
sf::TcpSocket Connection::socket;
uf::ThreadSafeQueue<Command *> Connection::commandQueue;
uptr<network::CommandsProcessor> Connection::syncCommandsProcessor;
