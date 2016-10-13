#include "network.hpp"

using namespace std;
using namespace sf;

usersDB::usersDB(string adr) {
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
bool usersDB::content(string &login, string &pass) {
	if (login == "") return false;
	return (all.count(login) == 1) && (all[login] == pass);
}
bool usersDB::add(string login, string pass) {
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


netclient::netclient(sf::TcpSocket* soc) {
	socket = soc;
}
result netclient::login(string &s) {
	std::istringstream ss(s);
	string login, pass;
	ss >> login;
	ss >> pass;
	//cout << login << endl;
	//cout << pass << endl;
	if (network::UBD.content(login, pass))
		return OK;
	else
		return LOGIN_ERROR;
}

result netclient::signin(string &s) {
	std::istringstream ss(s);
	string login, pass;
	ss >> login;
	ss >> pass;
	if (network::UBD.add(login, pass))
		return OK;
	else
		return SIGNIN_ERROR;
}


result netclient::parse(sf::Packet & pac) {
	string s;
	pac >> s;
	char code = s[0];
	s = &(s[1]);
	result res;
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


network::network(int port) {
	network::port = port;
	network::number_listeners = number_listeners;
	network::main_net = new thread(&network::listen);

}
void network::listen() {

	sf::TcpListener listener;
	listener.listen(port);
	sf::TcpSocket* client = new sf::TcpSocket;
	while (true) {
		sleep(seconds(0.1f));
		if (listener.accept(*client) == sf::TcpSocket::Done) {
			threads.push_back(thread(session, client));
			client = new sf::TcpSocket;
		}
	}
}

void network::session(sf::TcpSocket * client) {
	sf::Packet pac;
	netclient cl(client);
	while (true) {
		client->receive(pac);
		if (pac.getDataSize() > 0) {
			result rs = cl.parse(pac);
			pac.clear();
			pac << string{ (char)rs };
			client -> send(pac);
			pac.clear();
		}
		sleep(seconds(0.1f));
	}
}

network::~network() {
	for (auto i = threads.begin(); i != threads.end(); i++)
		i->~thread();
	main_net->~thread();
}

int network::number_listeners;
int network::port;
list<thread> network::threads;
thread * network::main_net;
usersDB network::UBD("usersDB");
/*
int main()
{
	network s;
	s.main_net->join();
	return 0;
}
*/

