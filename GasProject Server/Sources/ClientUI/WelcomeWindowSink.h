#pragma once

#include "WindowSink.h"

class WelcomeWindowSink : public WindowSink {
public:
	WelcomeWindowSink(Player *player);

	void OnTrigger(const std::string &trigger) override;
	void OnInput(const std::string &input, uptr<network::protocol::UIData> &&data) override;
};
