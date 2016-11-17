#pragma once

#include <string>

#include "Common/Useful.hpp"
#include "World/Camera.hpp"

using std::string;

namespace sf {
    class TcpSocket;
}

class Server;
class Game;
class Connection;
struct ServerCommand;

class Mob;

class Player {
private:
    string ckey;
    Server *server;
    Game *game;
    uptr<Connection> connection;

    Mob *mob;
    uptr<Camera> camera;

    ThreadSafeQueue<ServerCommand *> commandQueue;

public:
    Player(Server *server, sf::TcpSocket *socket);

    void Update();

    string GetCKey() { return ckey; }
    Connection *GetConnection() { return connection.get(); }

    void SetGame(Game *game) { this->game = game; }
    void SetMob(Mob *mob);
    void SetCamera(Camera *camera) { this->camera.reset(camera); }

    Mob *GetMob() { return mob; }
    Camera *GetCamera() { return camera.get(); }

    void AddCommand(ServerCommand *);

    ~Player();

    friend Connection;
};