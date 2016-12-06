#include "Server.hpp"
#include "Player.hpp"
//#include "Network/Network.hpp"
#include "World/World.hpp"
#include "PlayerCommand.hpp"
#include "Common/Ñommand.hpp"

class Server;

Player::Player() : ckey(""),
                                        game(nullptr)
{
    ckey = "";
    game = nullptr;
    connected = true;
}

void Player::Authorize(string login, string password) {
    if (Server::Get()->Authorization(string(login), string(password))) {
        ckey = string(login);
        commandsToClient.Push(new AuthSuccessServerCommand());
        commandsToClient.Push(new GameListServerCommand());
    } else {
        commandsToClient.Push(new AuthErrorServerCommand());
    }
}

void Player::Register(string login, string password) {
    if (Server::Get()->Registration(string(login), string(password)))
        commandsToClient.Push(new RegSuccessServerCommand());
    else
        commandsToClient.Push(new RegErrorServerCommand());
}

void Player::UpdateServerList() {
    commandsToClient.Push(new GameListServerCommand());
}

void Player::JoinToGame(int id) {
    if (Game *game = Server::Get()->JoinGame(id, this)) {
        this->game = game;
        commandsFromClient.Push(new JoinPlayerCommand);
        commandsToClient.Push(new GameJoinSuccessServerCommand());
    } else {
        commandsToClient.Push(new GameJoinErrorServerCommand());
    }
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