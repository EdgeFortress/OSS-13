#pragma once

#include <string>
#include <plog/Formatters/MessageOnlyFormatter.h>

#include <ResourceManager.hpp>
#include <Graphics/UI/Widget/LogRecordsHolder.h>

#include <Shared/Types.hpp>

using std::string;

class Window;
class UI;

class Player {
private:
    string pkey;
};

class ClientController :
	public plog::LogRecordsHolder<plog::MessageOnlyFormatter>
{
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
};

using CC = ClientController;