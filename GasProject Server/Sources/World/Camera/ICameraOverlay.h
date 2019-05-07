#pragma once

#include <string>

#include <SFML/System/Time.hpp>

#include <Shared/Network/Protocol/OverlayInfo.h>

class Tile;

class ICameraOverlay {
public:
	virtual ~ICameraOverlay() = default;

	virtual bool IsShouldBeUpdated(sf::Time timeElapsed) const = 0;
	virtual OverlayInfo GetOverlayInfo(Tile tile) const = 0;
};
