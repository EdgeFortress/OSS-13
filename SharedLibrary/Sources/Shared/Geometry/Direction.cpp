#include "Direction.hpp"

#include "Shared/Types.hpp"

namespace uf {

template<class T>
Direction VectToDirectionDetail(vec2<T> vector) {
	if (vector.x > 0) {
		if (vector.y > 0) return Direction::SOUTH_EAST;
		if (vector.y == 0) return Direction::EAST;
		if (vector.y < 0) return Direction::NORTH_EAST;
	}
	if (vector.x == 0) {
		if (vector.y > 0) return Direction::SOUTH;
		if (vector.y == 0) return Direction::NONE;
		if (vector.y < 0) return Direction::NORTH;
	}
	if (vector.x < 0) {
		if (vector.y > 0) return Direction::SOUTH_WEST;
		if (vector.y == 0) return Direction::WEST;
		if (vector.y < 0) return Direction::NORTH_WEST;
	}
	return Direction::NONE;
}

Direction VectToDirection(vec2f vector) { return VectToDirectionDetail(vector); }
Direction VectToDirection(vec3f vector) { return VectToDirectionDetail(vector.xy()); }

vec2i DirectionToVect(Direction direction) {
	switch (direction) {
		case Direction::SOUTH:
			return {0, 1};
		case Direction::WEST:
			return {-1, 0};
		case Direction::NORTH:
			return {0, -1};
		case Direction::EAST:
			return {1, 0};
		case Direction::SOUTH_WEST:
			return {-1, 1};
		case Direction::NORTH_WEST:
			return {-1, -1};
		case Direction::NORTH_EAST:
			return {1, -1};
		case Direction::SOUTH_EAST:
			return {1, 1};
		default:
			return {0, 0};
	}
}

Direction InvertDirection(Direction direction) {
	switch (direction) {
		case Direction::SOUTH:
			return Direction::NORTH;
		case Direction::WEST:
			return Direction::EAST;
		case Direction::NORTH:
			return Direction::SOUTH;
		case Direction::EAST:
			return Direction::WEST;
		case Direction::SOUTH_WEST:
			return Direction::NORTH_EAST;
		case Direction::NORTH_WEST:
			return Direction::SOUTH_EAST;
		case Direction::NORTH_EAST:
			return Direction::SOUTH_WEST;
		case Direction::SOUTH_EAST:
			return Direction::NORTH_WEST;
		default:
			return direction;
	}
}

bool SplitDirection(const Direction &input, Direction &first, Direction &second) {
	switch (input) {
		case Direction::SOUTH_WEST:
			first = Direction::SOUTH;
			second = Direction::WEST;
			return true;
		case Direction::NORTH_WEST:
			first = Direction::NORTH;
			second = Direction::WEST;
			return true;
		case Direction::NORTH_EAST:
			first = Direction::NORTH;
			second = Direction::EAST;
			return true;
		case Direction::SOUTH_EAST:
			first = Direction::SOUTH;
			second = Direction::EAST;
			return true;
		default:
			return false;
	}
}

}
