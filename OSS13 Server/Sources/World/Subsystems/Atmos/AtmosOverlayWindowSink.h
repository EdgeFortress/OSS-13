#pragma once

#include <ClientUI/WindowSink.h>

class AtmosCameraOverlay;

class AtmosOverlayWindowSink : public WindowSink {
public:
	AtmosOverlayWindowSink(Player *player);
	~AtmosOverlayWindowSink() final;

	void OnTrigger(const std::string &trigger) override;
	void OnInput(const std::string &input, uptr<network::protocol::UIData> &&data) override;

private:
	AtmosCameraOverlay *overlay;
};
