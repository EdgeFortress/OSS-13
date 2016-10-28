#include "Client.hpp"
#include "Window.hpp"
#include "State.hpp"
#include <net_const.hpp>

#include <iostream>

using std::cout;
using std::endl;

ClientController::ClientController() : player(new Player),
                                       window(new Window(this)),
                                       state(new GameProcessState(this)),
                                       newState(nullptr) {

}

void ClientController::Run() {
    if (!Connection::Start("localhost", PORT, this)) {
        cout << "Connection error!" << endl;
    } else {
        cout << "Connected" << endl;
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