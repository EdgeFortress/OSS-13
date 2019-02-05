#pragma once

#include <cstdint>

#include <SFML/System/Time.hpp>

#include <Shared/Global.hpp>

struct IconInfo {
	uint32_t id;
	std::string title;

	Global::ItemSpriteState state;

	bool isAnimation;
	sf::Time animation_time;
};
