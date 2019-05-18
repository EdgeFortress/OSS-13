#include "AtmosCameraOverlay.h"

#include <World/Tile.hpp>
#include <World/Atmos/Locale.hpp>

AtmosCameraOverlay::AtmosCameraOverlay() :
	mode(AtmosCameraOverlayMode::Locale)
{ }

void AtmosCameraOverlay::SetMode(AtmosCameraOverlayMode newMode) {
	mode = newMode;
}

bool AtmosCameraOverlay::IsShouldBeUpdated(sf::Time timeElapsed) const {
	timeAfterLastUpdate += timeElapsed;

	if (timeAfterLastUpdate > sf::seconds(1)) {
		timeAfterLastUpdate = sf::Time::Zero;
		return true;
	}

	return false;
}

using namespace network::protocol;

OverlayInfo AtmosCameraOverlay::GetOverlayInfo(Tile tile) const {
	OverlayInfo result;

	switch (mode) {
		case AtmosCameraOverlayMode::Locale: {
			auto locale = tile.GetLocale();
			if (locale) {
				auto localeID = tile.GetLocale()->ID();
				result.text = std::to_string(localeID);
			} else {
				result.text = "NULL";
			}
			break;
		}
		default: {
			result.text = "NULL";
			break;
		}
	}
	return result;
}
