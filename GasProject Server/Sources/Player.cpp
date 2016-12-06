#include "Server.hpp"
#include "Player.hpp"
#include "Network/Network.hpp"
#include "World/World.hpp"
#include "PlayerCommand.hpp"

class Server;

Player::Player(Server *server, sf::TcpSocket *socket) : ckey(""),
                                                        server(server),
                                                        game(nullptr),
                                                        connection(new Connection(socket, server, this))
{

}

void Player::Update() {
    while (!commandsFromClient.Empty()) {
        PlayerCommand *temp = commandsFromClient.Pop();
        switch (temp->GetCode()) {
            case PlayerCommand::Code::JOIN: {
                SetMob(game->GetWorld()->CreateNewPlayerMob());
                break;
            }
        }
        if (temp) delete temp;
    }

    if (mob->GetTile() != camera->GetPosition()) camera->SetPosition(mob->GetTile());
}

void Player::SendUpdates() {
    if (camera) {
        camera->UpdateView();
    }
}

void Player::SetMob(Mob *mob) {
    this->mob = mob;
    SetCamera(new Camera(mob->GetTile()));
    camera->SetPlayer(this);
};

void Player::AddCommandToClient(ServerCommand *command) {
    commandsToClient.Push(command);
}

void Player::AddCommandFromClient(PlayerCommand *command) {
    commandsFromClient.Push(command);
}

void Player::ChatMessage(std::wstring &message) {
    game->SendChatMessage(message, ckey);
    game->GetChat()->AddMessage(message, this);
}

Player::~Player() { connection->Stop(); }