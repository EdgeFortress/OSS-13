#include "Client.hpp"

#include <iostream>

#include <Shared/Global.hpp>

#include <Graphics/Window.hpp>
#include <Graphics/UI/UI.hpp>
#include <Graphics/Sprite.hpp>
#include <Network/Connection.h>

ClientController::ClientController(const std::filesystem::path& executablePath) :
    executablePath(executablePath),
    player(new Player),
    window(new Window())
{
    instance = this;
}

void ClientController::Run() {
	plog::init(plog::verbose, this);

	RM.Initialize();

    if (!Connection::Start()) {
		LOGE << "No connection established!";
    } else {
        LOGI << "Successfully connected to server.";
    };

    window->Initialize(executablePath);

    while (window->isOpen()) {
		Connection::ProcessSyncCommands();
        window->Update();
    }
    Connection::Stop();
}

Player *ClientController::GetClient() { return player.get(); }
Window *ClientController::GetWindow() { return window.get(); }
UI *ClientController::GetUI() { return window->GetUI(); }

ClientController *ClientController::Get() { return instance; }

const std::filesystem::path& ClientController::GetExecutablePath() { return executablePath; }

int main(int argc, char **argv) {
    auto executablePath = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path();

    auto clientController = std::make_unique<ClientController>(executablePath);
    clientController->Run();

    return 0;
}

ClientController *ClientController::instance;
