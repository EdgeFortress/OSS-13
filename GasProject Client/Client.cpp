#include "Client.hpp"

int main() {
	Network::Network();
	Network::connect();
	//Network::send_command(LOGIN_CODE, { "login", "pass" });
	ClientController clientController;
	
	return 0;
}
