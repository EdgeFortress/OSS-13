#pragma once

#include <string>
#include <filesystem>
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
    std::filesystem::path executablePath;

    uptr<Player> player;
    uptr<Window> window;
    static ClientController * instance;

public:
    ResourceManager RM;

    ClientController(const std::filesystem::path &executablePath);
    void Run();
    
    ClientController(const ClientController &) = delete;
    ClientController &operator=(const ClientController &) = delete;
    ~ClientController() = default;

	Player *GetClient();
	Window *GetWindow();
	UI *GetUI();
	static ClientController *Get();

    const std::filesystem::path& GetExecutablePath();
};

using CC = ClientController;
