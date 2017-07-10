#include "Server.hpp"
#include "Player.hpp"
#include "World/World.hpp"
#include "World/Objects/Control.hpp"
#include "World/Objects/Creature.hpp"
#include "PlayerCommand.hpp"
#include "Common/Ñommand.hpp"

class Server;

Player::Player() {
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

void Player::ChatMessage(std::string &message) {
    game->GetChat()->AddMessage("<" + ckey + ">" + message);
}

void Player::Move(Global::Direction direction) {
	commandsFromClient.Push(new MovePlayerCommand(Global::DirectionToVect(direction)));
}

void Player::Update() {
    while (!commandsFromClient.Empty()) {
        PlayerCommand *temp = commandsFromClient.Pop();
        if (temp) {
            switch (temp->GetCode()) {
                case PlayerCommand::Code::JOIN: {
					Creature *mob = game->GetWorld()->CreateNewPlayerCreature();
                    SetControl(mob->GetComponent<Control>());
                    break;
                }
				case PlayerCommand::Code::MOVE: {
					if (control) {
						MovePlayerCommand *moveCommand = dynamic_cast<MovePlayerCommand *>(temp);
						control->MoveCommand(moveCommand->order);
					}
				}
            }
            delete temp;
        }
    }

    if (control->GetOwner()->GetTile() != camera->GetPosition()) camera->SetPosition(control->GetOwner()->GetTile());
}

void Player::SendGraphicsUpdates() {
    if (camera) {
        camera->UpdateView();
    }
}

void Player::SetControl(Control *control) {
    this->control = control;
    SetCamera(new Camera(control->GetOwner()->GetTile()));
    camera->SetPlayer(this);
};

void Player::AddCommandToClient(ServerCommand *command) {
    commandsToClient.Push(command);
}

void Player::AddCommandFromClient(PlayerCommand *command) {
    commandsFromClient.Push(command);
}