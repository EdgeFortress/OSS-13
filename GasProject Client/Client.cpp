#include "Client.hpp"
#include "Window.hpp"
#include "State.hpp"

#include <iostream>
using std::cout;
using std::endl;

ClientController::ClientController() : player(new Player),
                                       window(new Window(this)),
                                       state(new MenuLoginState(this)),
                                       newState(nullptr) {

}

void ClientController::Run() {
    if (!Network::Connect("localhost", PORT, this)) {
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
}

int main() {
	ClientController clientController;
    clientController.Run();
	
	return 0;
}