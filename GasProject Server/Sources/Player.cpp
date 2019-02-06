#include "Player.hpp"

#include <Server.hpp>
#include <Network/Connection.hpp>
#include <World/World.hpp>
#include <World/Tile.hpp>
#include <World/Objects.hpp>

#include <Shared/Command.hpp>

class Server;

Player::Player(std::string ckey) : ckey(ckey) {
    game = nullptr;
	control = nullptr;
}

void Player::SetConnection(sptr<Connection> &connection) {
	this->connection = connection;
	if (camera && control) {
		camera->SetPosition(control->GetOwner()->GetTile());
	}
}

void Player::UpdateServerList() {
	if (sptr<Connection> connect = connection.lock())
		connect->commandsToClient.Push(new GameListServerCommand());
}

void Player::JoinToGame(Game *game) {
	this->game = game;
	actions.Push(new JoinPlayerCommand);
}

void Player::ChatMessage(std::string &message) {
    game->GetChat()->AddMessage("<" + ckey + ">" + message);
}

void Player::Move(uf::Direction direction) {
	actions.Push(new MovePlayerCommand(uf::DirectionToVect(direction)));
}

void Player::ClickObject(uint id) {
    actions.Push(new ClickObjectPlayerCommand(id));
}

void Player::Drop() {
	actions.Push(new DropPlayerCommand());
}

void Player::Build() {
	actions.Push(new BuildPlayerCommand());
}

void Player::Ghost() {
	actions.Push(new GhostPlayerCommand());
}

void Player::Update() {
    while (!actions.Empty()) {
        PlayerCommand *temp = actions.Pop();
        if (temp) {
            switch (temp->GetCode()) {
                case PlayerCommand::Code::JOIN: {
                    SetControl(game->GetStartControl(this));
                    break;
                }
				case PlayerCommand::Code::MOVE: {
					if (control) {
						MovePlayerCommand *moveCommand = dynamic_cast<MovePlayerCommand *>(temp);
						control->MoveCommand(moveCommand->order);
					}
					break;
				}
                case PlayerCommand::Code::CLICK_OBJECT: {
                    if (control) {
                        auto clickObjectPlayerCommand = dynamic_cast<ClickObjectPlayerCommand *>(temp);
                        control->ClickObjectCommand(clickObjectPlayerCommand->id);
                    }
                    break;
                }
				case PlayerCommand::Code::DROP: {
					if (!control) break;
					if (auto *creature = dynamic_cast<Creature *>(control->GetOwner()))
						creature->Drop();
				}
				case PlayerCommand::Code::BUILD: {
					if (!control) break;
					Tile *tile = control->GetOwner()->GetTile();
					if (tile)
						tile->PlaceTo(new Wall());
					break;
				}
				case PlayerCommand::Code::GHOST: {
					if (!control) break;
					::Ghost *ghost = dynamic_cast<::Ghost *>(control->GetOwner());
					if (!ghost) {
						ghost = new ::Ghost();
						control->GetOwner()->GetTile()->PlaceTo(ghost);
						ghost->SetHostControl(control);
						SetControl(ghost->GetComponent<Control>());
					} else {
						SetControl(ghost->GetHostControl());
                        ghost->Delete();
					}
					break;
				}
                default:
                    break;
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

void Player::Suspend() {
	camera->Suspend();
}

void Player::SetControl(Control *control) {
	if (this->control) this->control->player = nullptr;
    this->control = control;
	control->player = this;
    SetCamera(new Camera(control->GetOwner()->GetTile()));
    camera->SetPlayer(this);
	// Get Ability to see Invisibile from the mob (if control owner is mob)
	if (Creature *creature = dynamic_cast<Creature *>(control->GetOwner()))
		camera->SetInvisibleVisibility(creature->GetInvisibleVisibility());
};

void Player::SetCamera(Camera *camera) { this->camera.reset(camera); }

sptr<Connection> Player::GetConnection() {
	return connection.lock();
}

Control *Player::GetControl() { return control; }
Camera *Player::GetCamera() { return camera.get(); }
bool Player::IsConnected() { return !connection.expired(); }

void Player::AddCommandToClient(ServerCommand *command) {
	if (sptr<Connection> connect = connection.lock())
		connect->commandsToClient.Push(command);
}