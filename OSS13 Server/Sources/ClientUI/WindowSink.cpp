#include "WindowSink.h"

#include <Player/Player.h>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>

using namespace network::protocol;

WindowSink::WindowSink(Player *player, const std::string &id) :
	player(player),
	id(id),
	isClosed(false)
{ }

void WindowSink::Initialize() {
	auto command = std::make_unique<network::protocol::server::OpenWindowCommand>();

	command->id = id;
	for (auto &field : fields) {
		command->data.fields.push_back(field.second->Clone());
	}

	player->AddCommandToClient(command.release());
}

void WindowSink::Update(std::chrono::microseconds elapsed) { }

void WindowSink::OnTrigger(const std::string &trigger) {
	if (trigger == "Close") {
		isClosed = true;
	}
}

void WindowSink::OnInput(const std::string &input, uptr<UIData> &&data) {
	auto command = std::make_unique<network::protocol::server::UpdateWindowCommand>();
	command->data = data->Clone();
	player->AddCommandToClient(command.release());

	fields[input] = std::forward<uptr<UIData>>(data);
}

const std::string &WindowSink::Id() { return id; }
Player *WindowSink::GetPlayer() { return player; }
bool WindowSink::IsClosed() { return isClosed; }
