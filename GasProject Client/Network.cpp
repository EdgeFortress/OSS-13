#include <thread>

#include <SFML/Network.hpp>

#include "network.hpp"
#include "Client.hpp"
#include "Window.hpp"
#include "UI.hpp"

using namespace std;
using namespace sf;

bool Network::Connect(string ip, int port, ClientController *clientController) {
    Network::ip = ip;
    Network::port = port;
    Network::clientController = clientController;
    if (connected = (socket.connect(ip, port, seconds(1)) != sf::Socket::Done)) {
        return false;
    }
    Network::thread.reset(new std::thread(&session));
    return true;
}

void Network::session() {
    while (true) {
        bool working = false;
        if (!commandQueue.Empty()) {
            sendCommands();
            working = true;
        }
        if (needReceive && commandQueue.Empty()) {
            sf::Packet packet;
            socket.receive(packet);
            parsePacket(packet);
            working = true;
        }
        if (!working) sleep(seconds(0.01f));
    }
}

void Network::sendCommands() {
	while (!commandQueue.Empty()) {
		sf::Packet packet;
        packet << commandQueue.Front();
		socket.send(packet);
		commandQueue.Pop();
	}
}

void Network::parsePacket(Packet &packet) {
    sf::Int32 code;
    packet >> code;
    switch (code) {
        case ServerCommand::AUTH_SUCCESS:
            clientController->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(true);
            break;
        case ServerCommand::REG_SUCCESS: 
            clientController->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(true);
            break;
        case ServerCommand::AUTH_ERROR: 
            clientController->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(false);
            break;
        case ServerCommand::REG_ERROR:
            clientController->GetWindow()->GetUI()->GetAuthUI()->SetServerAnswer(false);
            break;
    };
}

Packet &operator<<(Packet &packet, ClientCommand *command) {
    packet << command->GetCode();
    switch (command->GetCode()) {
        case ClientCommand::AUTH_REQ: {
            auto c = dynamic_cast<AuthorizationClientCommand *>(command);
            packet << c->login << c->password;
            break;
        }
        case ClientCommand::REG_REQ: {
            auto c = dynamic_cast<RegistrationClientCommand *>(command);
            packet << c->login << c->password;
            break;
        }
    }
    return packet;
}

string Network::ip;
int Network::port;
ClientController *Network::clientController;
bool Network::connected = false;
bool Network::needReceive = false;
uptr<std::thread> Network::thread;
sf::TcpSocket Network::socket;
ThreadSafeQueue<ClientCommand *> Network::commandQueue;