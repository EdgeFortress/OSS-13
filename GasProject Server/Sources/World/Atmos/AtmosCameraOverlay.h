#pragma once

#include <World/Camera/ICameraOverlay.h>

#include <Shared/Network/Protocol/ServerToClient/OverlayInfo.h>

enum class AtmosCameraOverlayMode : char {
	None,
	Locale,
	Pressure,
	Temperature,
	PartialGasPressure
};

class AtmosCameraOverlay : public ICameraOverlay {
public:
	AtmosCameraOverlay();

	void SetMode(AtmosCameraOverlayMode mode);

// ICameraOverlay
	bool IsShouldBeUpdated(std::chrono::microseconds timeElapsed) const override;
	network::protocol::OverlayInfo GetOverlayInfo(Tile tile) const override;

private:
	mutable std::chrono::microseconds timeAfterLastUpdate{0};
	AtmosCameraOverlayMode mode;
};
