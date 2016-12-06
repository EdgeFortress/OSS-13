#pragma once

#include <string>

#include "Common/Useful.hpp"
#include "World/Camera.hpp"
#include "PlayerCommand.hpp"

using std::string;

class Server;
class Game;
class NetworkController;
struct ServerCommand;

class Mob;

class Player {
private:
    string ckey;
    bool connected;
    //Server *server;
    Game *game;
    //uptr<Connection> connection;

    Mob *mob;
    uptr<Camera> camera;

    ThreadSafeQueue<ServerCommand *> commandsToClient;
    ThreadSafeQueue<PlayerCommand *> commandsFromClient;

public:
    Player();

    // Client interface
    void Authorize(string login, string password);
    void Register(string login, string password);
    void UpdateServerList();
    void JoinToGame(int id);

    void Update();
    void SendUpdates();

    string GetCKey() { return ckey; }
    //Connection *GetConnection() { return connection.get(); }

    void SetMob(Mob *mob);
    void SetCamera(Camera *camera) { this->camera.reset(camera); }

    Mob *GetMob() { return mob; }
    Camera *GetCamera() { return camera.get(); }
    bool IsConnected() { return connected; }

    void AddCommandToClient(ServerCommand *);
    void AddCommandFromClient(PlayerCommand *);

    friend NetworkController;
    //friend Connection;
    friend Server;
};