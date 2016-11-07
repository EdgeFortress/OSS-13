#include "Player.hpp"
#include "Network/Network.hpp"
#include "World/World.hpp"

class Server;

Player::Player(Server *server, sf::TcpSocket *socket) : ckey(""),
                                                        server(server),
                                                        game(nullptr),
                                                        connection(new Connection(socket, server, this)) {

}

void Player::SetMob(Mob *mob) {
    this->mob = mob;
    SetCamera(new Camera(mob->GetTile()));
};

Player::~Player() { connection->Stop(); }