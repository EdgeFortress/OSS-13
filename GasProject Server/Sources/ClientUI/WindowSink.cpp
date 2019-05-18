#include "WindowSink.h"

#include <Player.hpp>
#include <Shared/Command.hpp>
#include <Shared/Network/Protocol/WindowData.h>

using namespace network::protocol;

WindowSink::WindowSink(Player *player, const std::string &id) :
	player(player),
	id(id),
	isClosed(false)
{ }

void WindowSink::Initialize() {
	WindowData data;
	for (auto &field : fields) {
		data.fields.push_back(field.second->Clone());
	}
	player->AddCommandToClient(new OpenWindowServerCommand(id, std::move(data)));
}

void WindowSink::Update(sf::Time elapsed) { }

void WindowSink::OnTrigger(const std::string &trigger) {
	if (trigger == "Close") {
		isClosed = true;
	}
}

void WindowSink::OnInput(const std::string &input, uptr<UIData> &&data) {
	player->AddCommandToClient(new UpdateWindowServerCommand(data->Clone()));
	fields[input] = std::forward<uptr<UIData>>(data);
}

const std::string &WindowSink::Id() { return id; }
Player *WindowSink::GetPlayer() { return player; }
bool WindowSink::IsClosed() { return isClosed; }
