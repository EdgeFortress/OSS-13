#pragma once

#include <string>
#include <chrono>

#include <Shared/Network/Protocol/OverlayInfo.h>

class Tile;

class ICameraOverlay {
public:
	virtual ~ICameraOverlay() = default;

	virtual bool IsShouldBeUpdated(std::chrono::microseconds timeElapsed) const = 0;
	virtual network::protocol::OverlayInfo GetOverlayInfo(Tile tile) const = 0;
};
