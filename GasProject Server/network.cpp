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

Result Netclient::Authorization(string &s) {
	std::istringstream ss(s);
	string login, pass;
	ss >> login;
	ss >> pass;
	cout << login << endl;
	cout << pass << endl;
	if (Network::UDB.contain(login, pass))
		return OK;
	else
		return AUTH_ERROR;
}

Result Netclient::Registration(string &s) {
	std::istringstream ss(s);
	string login, pass;
	ss >> login;
	ss >> pass;
	if (Network::UDB.add(login, pass))
		return OK;
	else
		return REG_ERROR;
}


Result Netclient::parse(sf::Packet &pac) {
	string s;
	pac >> s;
	char code = s[0];
	s = &(s[1]);
	Result res;
	switch (code) {
	case AUTH_CODE:
		res = Authorization(s);
		if (res == OK)
			logedin = true;
		else
			logedin = false;
		return res;
	case REG_CODE:
		return Registration(s);
	}
	return COMMAND_CODE_ERROR;
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
		Result rs = cl.parse(pac);
		pac.clear();
		pac << string{ (char)rs };
		socket->send(pac);
		pac.clear();
	}
}

bool Network::inProcess = false;
int Network::port;
list<thread *> Network::threads;
uptr<thread> Network::listeningThread;
UsersDB Network::UDB("usersDB");