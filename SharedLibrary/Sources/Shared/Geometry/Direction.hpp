#pragma once

namespace uf {
    template<typename T>
    struct vec2;

    typedef vec2<int> vec2i;

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

	Direction VectToDirection(vec2i);
	vec2i DirectionToVect(Direction);
}