#pragma once

#include <string>

#include <Shared/Network/Protocol/InputData.h>

class Player;

class WindowSink {
public:
	WindowSink(Player *player, const std::string &layout);

	virtual void OnTrigger(const std::string &trigger) = 0;
	virtual void OnInput(const std::string &input, uptr<network::protocol::UIData> &&data) = 0;

private:
	Player *player;
	const std::string &layout;
};
