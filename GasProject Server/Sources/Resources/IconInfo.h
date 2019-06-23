#pragma once

#include <cstdint>
#include <chrono>

#include <Shared/Global.hpp>

struct IconInfo {
	uint32_t id;
	std::string title;

	Global::ItemSpriteState state;

	bool isAnimation;
	std::chrono::microseconds animation_time;
};
