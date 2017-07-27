#include "Client.hpp"

#include "Graphics/Window.hpp"
#include "State.hpp"
#include "Network.hpp"
#include "Shared/Global.hpp"

#include <iostream>

ClientController::ClientController() : 
    player(new Player),
    window(new Window()),
    state(nullptr),
    newState(nullptr) 
{
    instance = this;
}

void ClientController::Run() {
    if (!Connection::Start("localhost", Global::PORT)) {
        CC::log << "Connection error!" << std::endl;
    } else {
        CC::log << "Connected" << std::endl;
    };

    SetState(new MenuLoginState);  

    window->Initialize();

    while (window->isOpen()) {
        window->Update();
        if (newState) {
            if (state) state->Ending();
            state.reset(newState);
            state->Initialize();
            newState = nullptr;
        }
    }
    Connection::Stop();
}

void ClientController::SetState(State *state) { newState = state; }

Player *ClientController::GetClient() { return player.get(); }
Window *ClientController::GetWindow() {
	if (this) return window.get();
	else return nullptr;
}
State *ClientController::GetState() { return state.get(); }
ClientController * const ClientController::Get() { return instance; }


int main() {
    ClientController clientController;
    clientController.Run();

    return 0;
}

uf::Log CC::log;
ClientController *ClientController::instance;