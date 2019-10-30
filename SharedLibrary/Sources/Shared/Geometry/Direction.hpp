#pragma once

#include "Angle.h"
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
	TOP,
	BOTTOM,
	CENTER,
	/// ...
	Count
};

constexpr int DIRECTION_COMPOSITE_COUNT = static_cast<int>(Direction::SOUTH_EAST) - static_cast<int>(Direction::SOUTH_WEST) + 1;
constexpr int DIRECTION_PURE_COUNT = static_cast<int>(Direction::Count) - DIRECTION_COMPOSITE_COUNT;

inline const std::initializer_list<Direction> PURE_DIRECTIONS_LIST = { Direction::SOUTH, Direction::WEST, Direction::NORTH, Direction::EAST, Direction::TOP, Direction::BOTTOM };

bool IsPureDirection(Direction direction);

Direction VectToDirection(vec2f);
Direction VectToDirection(vec3f);
vec3i DirectionToVect(Direction);
Angle DirectionToAngle(Direction);

Direction InvertDirection(Direction);
bool SplitDirection(const Direction &input, Direction &first, Direction &second);

}
