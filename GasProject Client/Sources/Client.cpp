#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "State.hpp"
#include "Common/NetworkConst.hpp"

#include <iostream>
//#include <ctime>

using std::endl;

ClientController::ClientController() : 
	player(new Player),
	window(new Window()),
	state(nullptr),
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
    //time_t startTime = time(nullptr);

    SetState(new MenuLoginState);

    while (window->isOpen()) {
        sf::Time timeElapsed = clock.restart();
        window->Update(timeElapsed);
        if (newState) {
            if (state) state->Ending();
            state.reset(newState);
            state->Initialize();
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