#include <SFML/Network.hpp>
#include "network.hpp"
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
	//Server LoH;
	Network s;
	s.main_net->join();

	return 0;
}