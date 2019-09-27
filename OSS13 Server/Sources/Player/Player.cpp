#include "Player.h"

#include <Shared/ErrorHandling.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>

#include <IGame.h>
#include <IScriptEngine.h>
#include <Network/Connection.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/Object.hpp>
#include <World/Objects/Control.hpp>
#include <ClientUI/WelcomeWindowSink.h>

#include "PlayerCommandsProcessor.h"

class Server;

using namespace std::string_literals;

Player::Player(std::string ckey) : 
	ckey(ckey),
	playerCommandsProcessor(std::make_unique<PlayerCommandsProcessor>(this))
{
	control = nullptr;
	AddVerb("spawn", &Player::OpenSpawnWindow);
}

void Player::SetConnection(sptr<Connection> &connection) {
	this->connection = connection;
	if (camera && control) {
		camera->SetPosition(control->GetOwner()->GetTile());
	}
}

void Player::updateUISinks(std::chrono::microseconds timeElapsed) {
	for (auto iter = uiSinks.begin(); iter != uiSinks.end();) {
		auto *sink = iter->second.get();
		sink->Update(timeElapsed);
		if (sink->IsClosed())
			iter = uiSinks.erase(iter);
		else
			iter++;
	}
}

void Player::Update(std::chrono::microseconds timeElapsed) {
	playerCommandsProcessor->ProcessCommands();

	if (justJoined) {
		justJoined = false;

		SetControl(GGame->GetStartControl(this));
		verbsHolders["player"] = this;
		verbsHolders["atmos"] = GGame->GetWorld()->GetMap()->GetAtmos();
		verbsHolders["creature"] = GetControl()->GetOwner();
	}

	camera->Update(timeElapsed);
	updateUISinks(timeElapsed);
}

void Player::SendGraphicsUpdates(std::chrono::microseconds timeElapsed) {
	if (camera)
		camera->UpdateView(timeElapsed);
}

void Player::OpenSpawnWindow() {
	AddCommandToClient(new network::protocol::server::OpenSpawnWindowCommand());
}

void Player::Suspend() {
	camera->Suspend();
}

void Player::SetControl(Control *control) {
	if (this->control) this->control->player = nullptr;
    this->control = control;
	control->player = this;
	control->GetUI()->NeedFullRefresh();
    SetCamera(new Camera(control->GetOwner()->GetTile()));
    camera->SetPlayer(this);
	camera->TrackObject(control->GetOwner());
	camera->SetInvisibleVisibility(control->GetSeeInvisibleAbility());
}

void Player::SetCamera(Camera *camera) { this->camera.reset(camera); }

sptr<Connection> Player::GetConnection() {
	return connection.lock();
}

Control *Player::GetControl() { return control; }
Camera *Player::GetCamera() { return camera.get(); }
bool Player::IsConnected() { return !connection.expired(); }

void Player::AddCommandToClient(network::protocol::Command *command) {
	if (sptr<Connection> connect = connection.lock())
		connect->commandsToClient.Push(command);
}

void Player::AddSyncCommandFromClient(uptr<network::protocol::Command> &&command) {
	playerCommandsProcessor->AddCommand(std::move(command));
}
