#include "Player.hpp"
#include "Network/Network.hpp"

class Server;

Player::Player(Server *server, sf::TcpSocket *socket) : ckey(""),
                                                        server(server),
                                                        game(nullptr),
                                                        connection(new Connection(socket, server, this)) {

}

Player::~Player() { connection->Stop(); }