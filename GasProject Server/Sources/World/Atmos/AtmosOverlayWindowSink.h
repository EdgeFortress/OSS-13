#pragma once

#include <ClientUI/WindowSink.h>

class AtmosOverlayWindowSink : public WindowSink {
public:
	AtmosOverlayWindowSink(Player *player);

	void OnTrigger(const std::string &trigger) override;
	void OnInput(const std::string &input, uptr<UIData> &&data) override;
};
