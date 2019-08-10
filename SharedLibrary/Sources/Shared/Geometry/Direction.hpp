#pragma once

namespace uf {
    template<typename T>
    struct vec2;
    template<typename T>
    struct vec3;

    typedef vec2<int> vec2i;
    typedef vec3<int> rpos;

	enum class Direction : char {
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

	Direction VectToDirection(vec2i);
	Direction VectToDirection(rpos);
	vec2i DirectionToVect(Direction);
}