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

	void UIInput(const std::string &handle, uptr<network::protocol::UIData> &&data);
	void CallVerb(const std::string &verb);
    ///

    void Update();
    void SendGraphicsUpdates(sf::Time timeElapsed);

	template <class T>
	void OpenWindow() {
		uiSinks.push_back(std::make_unique<T>(this));
	}

	void OpenWindow(const char *layout);

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
	std::string ckey;

	//bool connected;
	Game *game;

	Control *control;
	uptr<Camera> camera;

	wptr<Connection> connection;
	uf::ThreadSafeQueue<PlayerCommand *> actions;

	bool atmosOverlayToggled;

	std::vector<uptr<WindowSink>> uiSinks;
	std::map<std::string, const IVerbsHolder *> verbsHolders;
};
