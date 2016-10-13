#include "Client.hpp"
#include "network.cpp"

int main() {
	Network nt;
	nt.connect();
	nt.send_command(LOGIN_CODE, { "login", "pass" });
	ClientController clientController;
	
	return 0;
}
