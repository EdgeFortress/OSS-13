#include <thread>

#include <SFML/Network.hpp>

#include "network.hpp"
#include "Client.hpp"
#include "Window.hpp"
#include "UI.hpp"

using namespace std;
using namespace sf;

bool Connection::Start(string ip, int port) {
	status = WAITING;

    serverIp = ip;
    serverPort = port;
	Connection::thread.reset(new std::thread(&session));

	while (GetStatus() == WAITING) {
		sleep(seconds(0.01f));
	}
	if (GetStatus() == CONNECTED)
		return true;
	if (GetStatus() == NOT_CONNECTED)
		return false;
}

void Connection::Stop() {
	Connection::commandQueue.Push(new DisconnectionClientCommand());
	status = NOT_CONNECTED;
	thread->join();
}

void Connection::session() {
	if (socket.connect(serverIp, serverPort, seconds(5)) != sf::Socket::Done)
		status = NOT_CONNECTED;
	else
		status = CONNECTED;

	socket.setBlocking(false);

    while (status == CONNECTED) {
        bool working = false;
        if (!commandQueue.Empty()) {
            sendCommands();
            working = true;
        }
        sf::Packet packet;
		if (socket.receive(packet) == sf::Socket::Done) {
			parsePacket(packet);
			working = true;
		}
        if (!working) sleep(seconds(0.01f));
    }
	if (!commandQueue.Empty())
		sendCommands();
}

void Connection::sendCommands() {
	while (!commandQueue.Empty()) {
		sf::Packet packet;
		ClientCommand *temp = commandQueue.Pop();
		packet << temp;
		if (temp) delete temp;
		while (socket.send(packet) == sf::Socket::Partial);
	}
}

void Connection::parsePacket(Packet &packet) {
    sf::Int32 code;
    packet >> code;
    switch (code) {
        case ServerCommand::AUTH_SUCCESS:
			ClientController::Get()->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(true);
            break;
        case ServerCommand::REG_SUCCESS: 
			ClientController::Get()->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(true);
            break;
        case ServerCommand::AUTH_ERROR: 
			ClientController::Get()->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(false);
            break;
        case ServerCommand::REG_ERROR:
			ClientController::Get()->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(false);
            break;
    };
}

Packet &operator<<(Packet &packet, ClientCommand *command) {
    packet << sf::Int32(command->GetCode());
    switch (command->GetCode()) {
        case ClientCommand::AUTH_REQ: {
            auto c = dynamic_cast<AuthorizationClientCommand *>(command);
            packet << sf::String(c->login) << sf::String(c->password);
            break;
        }
        case ClientCommand::REG_REQ: {
            auto c = dynamic_cast<RegistrationClientCommand *>(command);
            packet << sf::String(c->login) << sf::String(c->password);
            break;
        }
    }
    return packet;
}

sf::IpAddress Connection::serverIp;
int Connection::serverPort;
Connection::Status Connection::status = INACTIVE;
ClientController *Connection::clientController;
uptr<std::thread> Connection::thread;
sf::TcpSocket Connection::socket;
ThreadSafeQueue<ClientCommand *> Connection::commandQueue;