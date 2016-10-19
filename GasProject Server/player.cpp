#include "player.hpp"
#include "network.hpp"

class Server;

Player::Player(Server *server, sf::TcpSocket *socket) : ckey(""),
                                                        server(server),
                                                        connection(new Connection(socket, server, this)) {

}