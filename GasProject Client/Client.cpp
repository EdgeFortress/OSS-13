#include "Client.hpp"
#include "network.cpp"

int main() {
	network nt;
	nt.connect();
	nt.send_command(login_code, { "login", "pass" });
	ClientController clientController;
	
	return 0;
}
