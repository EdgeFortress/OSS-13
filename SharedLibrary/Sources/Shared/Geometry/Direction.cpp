#include "Direction.hpp"

uf::Direction uf::VectToDirection(uf::vec2f vector) {
	if (vector.x > 0) {
		if (vector.y > 0)  return uf::Direction::SOUTH_EAST;
		if (vector.y == 0) return uf::Direction::EAST;
		if (vector.y < 0)  return uf::Direction::NORTH_EAST;
	}
	if (vector.x == 0) {
		if (vector.y > 0)  return uf::Direction::SOUTH;
		if (vector.y == 0) return uf::Direction::NONE;
		if (vector.y < 0)  return uf::Direction::NORTH;
	}
	if (vector.x < 0) {
		if (vector.y > 0)  return uf::Direction::SOUTH_WEST;
		if (vector.y == 0) return uf::Direction::WEST;
		if (vector.y < 0)  return uf::Direction::NORTH_WEST;
	}
	return uf::Direction::NONE;
}

uf::vec2f uf::DirectionToVect(uf::Direction direction) {
	switch (direction) {
	case uf::Direction::SOUTH:
		return uf::vec2f(0, 1);
	case uf::Direction::WEST:
		return uf::vec2f(-1, 0);
	case uf::Direction::NORTH:
		return uf::vec2f(0, -1);
	case uf::Direction::EAST:
		return uf::vec2f(1, 0);
	case uf::Direction::SOUTH_WEST:
		return uf::vec2f(-1, 1);
	case uf::Direction::NORTH_WEST:
		return uf::vec2f(-1, -1);
	case uf::Direction::NORTH_EAST:
		return uf::vec2f(1, -1);
	case uf::Direction::SOUTH_EAST:
		return uf::vec2f(1, 1);
	}
	return uf::vec2f(0, 0);
}