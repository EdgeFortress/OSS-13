#include "Direction.hpp"

namespace uf {
	Direction VectToDirection(vec2f vector) {
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

	vec2f DirectionToVect(Direction direction) {
		switch (direction) {
			case Direction::SOUTH:
				return vec2f(0, 1);
			case Direction::WEST:
				return vec2f(-1, 0);
			case Direction::NORTH:
				return vec2f(0, -1);
			case Direction::EAST:
				return vec2f(1, 0);
			case Direction::SOUTH_WEST:
				return vec2f(-1, 1);
			case Direction::NORTH_WEST:
				return vec2f(-1, -1);
			case Direction::NORTH_EAST:
				return vec2f(1, -1);
			case Direction::SOUTH_EAST:
				return vec2f(1, 1);
		}
		return vec2f(0, 0);
	}
}