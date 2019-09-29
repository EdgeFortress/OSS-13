#pragma once

#include "Vec2.hpp"
#include "Vec3.hpp"

namespace uf {

enum class Direction : int {
	NONE = -1,
	SOUTH,
	WEST,
	NORTH,
	EAST,
	SOUTH_WEST,
	NORTH_WEST,
	NORTH_EAST,
	SOUTH_EAST,
	CENTER
};

Direction VectToDirection(vec2f);
Direction VectToDirection(vec3f);
vec2i DirectionToVect(Direction);

Direction InvertDirection(Direction);
bool SplitDirection(const Direction &input, Direction &first, Direction &second);

}
