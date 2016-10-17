#include "Server.hpp"
#include "network.hpp"

using namespace std;
using namespace sf;

UsersDB::UsersDB(string adr) {
	this->adr = adr;
	ifstream file;
	file.open(adr, ios::in);
	while (file) {
		string login, pass;
		file >> login;
		file >> pass;
		all[login] = pass;
	}
	file.close();
}

bool UsersDB::Contain(string &login, string &pass) {
	if (login == "") return false;
	return (all.count(login) == 1) && (all[login] == pass);
}

bool UsersDB::Add(string login, string pass) {
	ofstream file;
	file.open(adr, ios::app);
	if (all.count(login) == 0) {
		all[login] = pass;
		file << login << " " << pass << endl;
		return true;
	} else
		return false;
	file.close();
}

Netclient::Netclient(uptr<sf::TcpSocket> &socket) : socket(socket) {
	
}

bool Netclient::Authorization(string &login, string &password) {
	if (Network::UDB.Contain(login, password))
	{
		cout << "Player is authorized: " << login << ' ' << password << endl;
		return true;
	}
	cout << "Wrong login data received: " << login << ' ' << password << endl;
	return false;
}

bool Netclient::Registration(string &login, string &password) {
	if (Network::UDB.Add(login, password)) { 
		cout << "New player is registrated: " << login << ' ' << password << endl;
		return true;
	}
	return false;
}

void Netclient::Parse(sf::Packet &pac) {
	int code;
	pac >> code;
	
    switch (code) {
        case ClientCommand::AUTH_REQ: {
            string login, password;
            pac >> login >> password;
            if (logedin = Authorization(login, password))
                Network::commandQueue.Push(new AuthSuccessServerCommand());
            else
                Network::commandQueue.Push(new AuthErrorServerCommand());
            break;
        }
        case ClientCommand::REG_REQ: {
            string login, password;
            pac >> login >> password;
            if (Registration(login, password))
                Network::commandQueue.Push(new RegSuccessServerCommand());
            else
                Network::commandQueue.Push(new RegErrorServerCommand());
            break;
        }
        default:
            Network::commandQueue.Push(new CommandCodeErrorServerCommand());
    }
}

void Network::Initialize(const int port) {
	if (!inProcess) {
		Network::port = port;
		Network::listeningThread.reset(new thread(&Network::listen));
		inProcess = true;
	}
}

void Network::WIP_Wait() {
	if (listeningThread)
		listeningThread->join();
}

void Network::listen() {
	sf::TcpListener listener;
	listener.listen(port);
	sf::TcpSocket *socket = new sf::TcpSocket;
	while (true) {
		if (listener.accept(*socket) == sf::TcpSocket::Done) {
			threads.push_back(new thread(clientSession, socket));
			socket = new sf::TcpSocket;
		} else {
			break;
		}
	}
	delete socket;
}

void Network::clientSession(sf::TcpSocket *raw_pointer_socket) {
	uptr<sf::TcpSocket> socket(raw_pointer_socket);
	cout << "New client is connected" << endl;
	sf::Packet packet;
	Netclient client(socket);
	while (true) {
		socket->receive(packet);
		client.Parse(packet);
		packet.clear();
		while (!commandQueue.Empty()) {
			ServerCommand *temp = commandQueue.Pop();
			packet << temp;
			if (temp) delete temp;
			socket->send(packet);
			packet.clear();
		}
	}
}

Packet &operator<<(Packet &packet, ServerCommand *serverCommand) {
	packet << sf::Int32(serverCommand->GetCode());
	return packet;
}

bool Network::inProcess = false;
int Network::port;
list<thread *> Network::threads;
uptr<thread> Network::listeningThread;
UsersDB Network::UDB("usersDB");
ThreadSafeQueue<ServerCommand *> Network::commandQueue;