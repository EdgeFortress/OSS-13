#pragma once

#include <string>

#include "Shared/Types.hpp"
#include "Shared/ThreadSafeQueue.hpp"
#include "World/Camera.hpp"
#include "PlayerCommand.hpp"

using std::string;

class Server;
class Game;
class NetworkController;
struct Connection;
struct ServerCommand;

class Control;

class Player {
private:
    string ckey;
    //bool connected;
    Game *game;

    Control *control;
    uptr<Camera> camera;

	wptr<Connection> connection;
    uf::ThreadSafeQueue<PlayerCommand *> actions;

public:
	explicit Player(std::string ckey);

	void SetConnection(sptr<Connection> &connection);

    /// Client interface
    void UpdateServerList();
    void JoinToGame(Game *game);
    void ChatMessage(std::string &message);

    void Move(uf::Direction);
	void Build();
	void Ghost();
    ///

    void Update();
    void SendGraphicsUpdates();

    string GetCKey() { return ckey; }

	void Suspend();
    void SetControl(Control *control);
	void SetCamera(Camera *camera);

	sptr<Connection> GetConnection();
	Control *GetControl();
	Camera *GetCamera();
	bool IsConnected();

    void AddCommandToClient(ServerCommand *);

    friend NetworkController;
    friend Server;
};