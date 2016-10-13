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
	if (network::UBD.content(login, pass))
		return ok;
	else
		return login_error;
}

result netclient::signin(string &s) {
	std::istringstream ss(s);
	string login, pass;
	ss >> login;
	ss >> pass;
	if (network::UBD.add(login, pass))
		return ok;
	else
		return signin_error;
}


result netclient::parse(sf::Packet & pac) {
	string s;
	pac >> s;
	char code = s[0];
	s = &(s[1]);
	result res;
	switch (code)
	{
	case login_code:
		res = login(s);
		if (res == ok)
			logedin = true;
		else
			logedin = false;
		return res;
	case singin_code:
		return signin(s);
	}
	return command_code_error;
}


network::network(int number_listeners, int start_port) {
	network::start_port = start_port;
	network::number_listeners = number_listeners;
	network::main_net = new thread(&network::start_all_listeners);

}
void network::start_all_listeners() {

	for (int i = 0; i < number_listeners; i++)
	{
		thread *  my_th = new thread(&network::session, start_port + i);
		threads.push_back(my_th);
	}
	for (auto i = threads.begin(); i != threads.end(); i++) {
		(*i)->join();
	}
}

void network::session(int port) {
	//cout << client -> getRemoteAddress() << ":" << client -> getRemotePort() << endl;
	sf::Packet pac;
	sf::TcpListener listener;
	if (listener.listen(port) != sf::Socket::Done)
	{
		// error...
		cout << "error listen on port " << port;
	}

	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done)
	{
		cout << "error accept on port " << port;
	}
	bool logged_in = false;

	netclient cl(&client);
	while (true) {
		sleep(seconds(0.01f));
		client.receive(pac);
		if (pac.getDataSize() != 0) {
			result rs = cl.parse(pac);
			pac.clear();
			pac << string{ (char)rs };
			client.send(pac);
			pac.clear();
		}
	}
}

network::~network() {
	for (auto i = threads.begin(); i != threads.end(); i++)
		(*i)->~thread();
	main_net ->~thread();
}

int network::number_listeners;
int network::start_port;
list<thread *> network::threads;
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

