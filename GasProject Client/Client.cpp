#include "Client.hpp"
#include "Window.hpp"
#include "State.hpp"
#include <net_const.hpp>

#include <iostream>

using std::endl;

ClientController::ClientController() : 
	player(new Player),
	window(new Window),
	state(new GameProcessState()),
	newState(nullptr) 
{
	instance = this;
}

void ClientController::Run() {
	window->Initilize();
    if (!Connection::Start("localhost", PORT)) {
        CC::log << "Connection error!" << endl;
    } else {
		CC::log << "Connected" << endl;
    };
    sf::Clock clock;

    while (window->isOpen()) {
        sf::Time timeElapsed = clock.restart();
        window->Update(timeElapsed);
        if (newState) {
            state.reset(newState);
            newState = nullptr;
        }
    }
	Connection::Stop();
}


int main() {
	ClientController clientController;
    clientController.Run();
	
	return 0;
}

const Log CC::log;
ClientController *ClientController::instance;