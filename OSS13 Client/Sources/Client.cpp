#include "Client.hpp"

#include <iostream>

#include <Shared/Global.hpp>

#include <Graphics/Window.hpp>
#include <Graphics/UI/UI.hpp>
#include <Graphics/Sprite.hpp>
#include <Network/Connection.h>

ClientController::ClientController() : 
    player(new Player),
    window(new Window())
{
    instance = this;
}

void ClientController::Run() {
	plog::init(plog::verbose, this);

	RM.Initialize();

    if (!Connection::Start("localhost", Global::PORT)) {
		LOGE << "No connection established!";
    } else {
        LOGI << "Successfully connected to server.";
    };

    window->Initialize();

    while (window->isOpen()) {
		Connection::ProcessSyncCommands();
        window->Update();
    }
    Connection::Stop();
}

Player *ClientController::GetClient() { return player.get(); }
Window *ClientController::GetWindow() { return window.get(); }
UI *ClientController::GetUI() { return window->GetUI(); }

ClientController * const ClientController::Get() { return instance; }

int main() {
    ClientController clientController;
    clientController.Run();

    return 0;
}

ClientController *ClientController::instance;
