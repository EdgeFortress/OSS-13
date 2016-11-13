#include "Player.hpp"
#include "Network/Network.hpp"
#include "World/World.hpp"

class Server;

Player::Player(Server *server, sf::TcpSocket *socket) : ckey(""),
                                                        server(server),
                                                        game(nullptr),
                                                        connection(new Connection(socket, server, this)) {

}

void Player::Update() {
    const auto &&differences = camera->GetVisibleDifferences();
    if (!differences.empty())
        commandQueue.Push(new GraphicsDiffsServerCommand(differences));
}

void Player::SetMob(Mob *mob) {
    this->mob = mob;
    SetCamera(new Camera(mob->GetTile()));
};

void Player::AddCommand(ServerCommand *command) {
    commandQueue.Push(command);
}

Player::~Player() { connection->Stop(); }