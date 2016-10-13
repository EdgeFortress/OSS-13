#include <SFML/Network.hpp>
#include "network.cpp"
#include "World.hpp"

class Server {
public:
	Server();
};

Server::Server()
{
	World world;
	world.FillingWorld();	
}

int main() {
	Server LoH;

	return 0;
}