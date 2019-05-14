#pragma once

#include "WindowSink.h"

class WelcomeWindowSink : public WindowSink {
public:
	WelcomeWindowSink(Player *player, const std::string &layout);

	void OnTrigger(const std::string &trigger) override;
	void OnInput(const std::string &input, uptr<UIData> &&data) override;
};
