#pragma once

#include <SFML/System/Time.hpp>

#include <World/Camera/ICameraOverlay.h>

#include <Shared/Network/Protocol/OverlayInfo.h>

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
	bool IsShouldBeUpdated(sf::Time timeElapsed) const override;
	OverlayInfo GetOverlayInfo(Tile tile) const override;

private:
	mutable sf::Time timeAfterLastUpdate;
	AtmosCameraOverlayMode mode;
};
