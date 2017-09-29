#pragma once

#include <string>

#include "Shared/Types.hpp"
#include "Shared/Log.hpp"
#include "ResourceManager.hpp"

using std::string;

class Window;
class UI;

class Player {
private:
    string pkey;
};

class ClientController {
private:
    uptr<Player> player;
    uptr<Window> window;
    static ClientController * instance;

public:
    ResourceManager RM;

    ClientController();
    void Run();
    
    ClientController(const ClientController &) = delete;
    ClientController &operator=(const ClientController &) = delete;
    ~ClientController() = default;

	Player *GetClient();
	Window *GetWindow();
	UI *GetUI();
	static ClientController *const Get();

    static uf::Log log;
};

using CC = ClientController;