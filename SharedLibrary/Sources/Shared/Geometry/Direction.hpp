#pragma once

namespace uf {

template<typename T>
struct vec2;
template<typename T>
struct vec3;

typedef vec2<int> vec2i;
typedef vec2<float> vec2f;
typedef vec3<int> rpos;

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
Direction VectToDirection(rpos);
vec2i DirectionToVect(Direction);

Direction InvertDirection(Direction);
bool SplitDirection(const Direction &input, Direction &first, Direction &second);

}
