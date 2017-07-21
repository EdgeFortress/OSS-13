#pragma once

#include "Vec2.hpp"

namespace uf {
	enum class Direction : char {
		NONE = -1,
		SOUTH,
		WEST,
		NORTH,
		EAST,
		SOUTH_WEST,
		NORTH_WEST,
		NORTH_EAST,
		SOUTH_EAST
	};

	Direction VectToDirection(vec2f);
	vec2f DirectionToVect(Direction);
}