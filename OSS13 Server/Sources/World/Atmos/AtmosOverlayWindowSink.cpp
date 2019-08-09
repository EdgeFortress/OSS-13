#include "AtmosOverlayWindowSink.h"

#include <Shared/Network/Protocol/InputData.h>
#include <Shared/ErrorHandling.h>

#include <Player.hpp>
#include <World/Camera/Camera.hpp>

#include "AtmosCameraOverlay.h"

using namespace network::protocol;

AtmosOverlayWindowSink::AtmosOverlayWindowSink(Player *player) :
	WindowSink(player, "AtmosOverlayWindow")
{
	auto mode = std::make_unique<RadioButtonUIData>();
	mode->data = 1;
	mode->handle = "Mode";
	fields["Mode"] = std::move(mode);

	Camera *camera = player->GetCamera();
	EXPECT(camera);

	auto overlay = std::make_unique<AtmosCameraOverlay>();
	this->overlay = overlay.get();
	camera->SetOverlay(std::move(overlay));
}

AtmosOverlayWindowSink::~AtmosOverlayWindowSink() {
	if (Camera *camera = GetPlayer()->GetCamera()) {
		camera->ResetOverlay();
	}
}

void AtmosOverlayWindowSink::OnTrigger(const std::string &trigger) {
	WindowSink::OnTrigger(trigger);
}

void AtmosOverlayWindowSink::OnInput(const std::string &input, uptr<UIData> &&data) {
	if (input == "Mode") {
		auto *p = dynamic_cast<RadioButtonUIData *>(data.get());
		auto mode = static_cast<AtmosCameraOverlayMode>(p->data);
		overlay->SetMode(mode);
	}
	WindowSink::OnInput(input, std::forward<uptr<UIData>>(data));
}
