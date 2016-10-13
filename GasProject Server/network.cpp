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
bool UsersDB::content(string &login, string &pass) {
	if (login == "") return false;
	return (all.count(login) == 1) && (all[login] == pass);
}
bool UsersDB::add(string login, string pass) {
	ofstream file;
	file.open(adr, ios::app);
	if (all.count(login) == 0) {
		all[login] = pass;
		file << login << " " << pass << endl;
		return true;
	}
	else
		return false;
}


Netclient::Netclient(sf::TcpSocket* soc) {
	socket = soc;
}
Result Netclient::login(string &s) {
	std::istringstream ss(s);
	string login, pass;
	ss >> login;
	ss >> pass;
	//cout << login << endl;
	//cout << pass << endl;
	if (Network::UBD.content(login, pass))
		return OK;
	else
		return LOGIN_ERROR;
}

Result Netclient::signin(string &s) {
	std::istringstream ss(s);
	string login, pass;
	ss >> login;
	ss >> pass;
	if (Network::UBD.add(login, pass))
		return OK;
	else
		return SIGNIN_ERROR;
}


Result Netclient::parse(sf::Packet & pac) {
	string s;
	pac >> s;
	char code = s[0];
	s = &(s[1]);
	Result res;
	switch (code)
	{
	case LOGIN_CODE:
		res = login(s);
		if (res == OK)
			logedin = true;
		else
			logedin = false;
		return res;
	case SIGNIN_CODE:
		return signin(s);
	}
	return COMMAND_CODE_ERROR;
}


Network::Network(int port) {
	Network::port = port;
	Network::main_net = new thread(&Network::listen);

}
void Network::listen() {

	sf::TcpListener listener;
	listener.listen(port);
	sf::TcpSocket* client = new sf::TcpSocket;
	while (true) {
		sleep(seconds(0.1f));
		if (listener.accept(*client) == sf::TcpSocket::Done) {
			threads.push_back(new thread(session, client));
			client = new sf::TcpSocket;
		}
	}
}

void Network::session(sf::TcpSocket * client) {
	sf::Packet pac;
	Netclient cl(client);
	while (true) {
		client->receive(pac);
		if (pac.getDataSize() > 0) {
			Result rs = cl.parse(pac);
			pac.clear();
			pac << string{ (char)rs };
			client -> send(pac);
			pac.clear();
		}
		sleep(seconds(0.1f));
	}
}

Network::~Network() {
	for (auto i = threads.begin(); i != threads.end(); i++)
		delete(*i);
	delete(main_net);
}

int Network::port;
list<thread*> Network::threads;
thread * Network::main_net;
UsersDB Network::UBD("usersDB");
/*
int main()
{
	Network s;
	s.main_net->join();
	return 0;
}
*/

