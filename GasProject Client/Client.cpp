#include "Client.hpp"

int main() {
	Network::Connect("localhost", PORT);
	ClientController clientController;
	
	return 0;
}