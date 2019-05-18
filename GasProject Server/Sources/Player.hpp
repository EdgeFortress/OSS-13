#pragma once

#include <string>

#include <PlayerCommand.hpp>
#include <ClientUI/WindowSink.h>
#include <World/Camera/Camera.hpp>

#include <Shared/Types.hpp>
#include <Shared/ThreadSafeQueue.hpp>
#include <Shared/Network/Protocol/InputData.h>

#include <SFML/System/Time.hpp>

#include "VerbsHolder.h"

class Server;
class Game;
class NetworkController;
struct Connection;
struct ServerCommand;

class Control;

class Player {
friend NetworkController;
friend Server;

public:
	explicit Player(std::string ckey);

	void SetConnection(sptr<Connection> &connection);

    /// Client interface
    void UpdateServerList();
    void JoinToGame(Game *game);
    void ChatMessage(std::string &message);

    void Move(uf::Direction);
    void MoveZ(bool up);
    void ClickObject(uint id);
	void Drop();
	void Build();
	void Ghost();

	void UIInput(uptr<network::protocol::UIData> &&data);
	void UITrigger(const std::string &window, const std::string &trigger);
	void CallVerb(const std::string &verb);
    ///

    void Update(sf::Time timeElapsed);
    void SendGraphicsUpdates(sf::Time timeElapsed);

	template <class T>
	void OpenWindow() {
		auto window = std::make_unique<T>(this);
		window->Initialize();
		uiSinks[window->Id()] = std::move(window);
	}

    std::string GetCKey() { return ckey; }

	void Suspend();
    void SetControl(Control *control);
	void SetCamera(Camera *camera);

	sptr<Connection> GetConnection();
	Control *GetControl();
	Camera *GetCamera();
	bool IsConnected();

    void AddCommandToClient(ServerCommand *);

private:
	void updateUISinks(sf::Time timeElapsed);

private:
	std::string ckey;

	//bool connected;
	Game *game;

	Control *control;
	uptr<Camera> camera;

	wptr<Connection> connection;
	uf::ThreadSafeQueue<PlayerCommand *> actions;

	bool atmosOverlayToggled;

	std::map<std::string, uptr<WindowSink>> uiSinks;
	std::map<std::string, const IVerbsHolder *> verbsHolders;
};
