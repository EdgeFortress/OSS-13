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
}
bool UsersDB::contain(string &login, string &pass) {
	if (login == "") return false;
	return (all.count(login) == 1) && (all[login] == pass);
}
bool UsersDB::add(string login, string pass) {
	ofstream file;
	file.open(adr, ios::ate);
	if (all.count(login) == 0) {
		all[login] = pass;
		file << login << " " << pass << endl;
		return true;
	}
	else
		return false;
}


Netclient::Netclient(uptr<sf::TcpSocket> &socket) : socket(socket) {
	
}

void Netclient::Authorization(string &login, string &password) {
	cout << login << endl;
	cout << password << endl;
	if (Network::UDB.contain(login, password))
		Network::commandQueue.Push(new SuccessServerCommand());
	else
		Network::commandQueue.Push(new AuthErrorServerCommand());
}

void Netclient::Registration(string &login, string &password) {
	if (Network::UDB.add(login, password))
		Network::commandQueue.Push(new SuccessServerCommand());
	else
		Network::commandQueue.Push(new RegErrorServerCommand());
}

void Netclient::parse(sf::Packet &pac) {
	int code;
	pac >> code;
	Result res;
	string login, password;
	switch (code) {
	case ClientCommand::AUTH_REQ:
		pac >> login >> password;
		Authorization(login, password);
		if (!(Network::commandQueue.Front()->GetCode()))
			logedin = true;
		else
			logedin = false;
		return;
	case ClientCommand::REG_REQ:
		pac >> login >> password;
		Registration(login, password);
		return;
	}
	Network::commandQueue.Push(new CommandCodeErrorServerCommand());
	return;
}

void Network::Initialize(const int port) {
	if (!inProcess) {
		Network::port = port;
		Network::listeningThread.reset(new thread(&Network::listen));
		inProcess = true;
	}
}

void Network::WIP_Wait() {
	listeningThread->join();
}

void Network::listen() {
	sf::TcpListener listener;
	listener.listen(port);
	sf::TcpSocket *socket = new sf::TcpSocket;
	while (true) {
		if (listener.accept(*socket) == sf::TcpSocket::Done) {
			threads.push_back(new thread(session, socket));
			socket = new sf::TcpSocket;
		}
	}
}

void Network::session(sf::TcpSocket *raw_pointer_socket) {
	uptr<sf::TcpSocket> socket(raw_pointer_socket);
	sf::Packet pac;
	Netclient cl(socket);
	while (true) {
		socket->receive(pac);
		cl.parse(pac);
		pac.clear();
		while (!commandQueue.Empty()) {
			pac << commandQueue.Front()->GetCode();
			socket->send(pac);
			commandQueue.Pop();
			pac.clear();
		}
	}
}

bool Network::inProcess = false;
int Network::port;
list<thread *> Network::threads;
uptr<thread> Network::listeningThread;
UsersDB Network::UDB("usersDB");
ThreadSafeQueue<ServerCommand *> Network::commandQueue;