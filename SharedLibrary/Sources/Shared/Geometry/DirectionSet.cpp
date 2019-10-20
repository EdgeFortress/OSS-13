#include "DirectionSet.h"

namespace uf {

constexpr size_t index(Direction direction) {
	switch (direction) {
		case Direction::SOUTH:
		case Direction::WEST:
		case Direction::NORTH:
		case Direction::EAST:
			return static_cast<size_t>(direction);

		case Direction::TOP:
		case Direction::BOTTOM:
		case Direction::CENTER:
			return static_cast<size_t>(direction) - DIRECTION_COMPOSITE_COUNT;

		default:
			break;
	}

	EXPECT_WITH_MSG(false, "Wrong direction received: " + std::to_string(static_cast<int>(direction)));
}

DirectionSet::DirectionSet(std::list<Direction> directions) {
	Add(directions);
}

void DirectionSet::Add(DirectionSet directions) {
	buffer |= directions.buffer;
}

void DirectionSet::Add(const std::list<Direction> &directions) {
	for (auto direction : directions) {
		if (direction == Direction::NONE)
			continue;
		Direction first, second;
		if (SplitDirection(direction, first, second)) {
			buffer[index(first)] = true;
			buffer[index(second)] = true;
		} else {
			buffer[index(direction)] = true;
		}
	}
}

void DirectionSet::Remove(DirectionSet directions) {
	buffer &= ~directions.buffer;
}

void DirectionSet::Remove(const std::list<Direction> &directions) {
	Remove(DirectionSet(directions));
}

bool DirectionSet::DoesExistOne(DirectionSet directions) const {
	return (buffer & directions.buffer).any();
}

bool DirectionSet::DoesExistOne(const std::list<Direction> &directions) const {
	return DoesExistOne(DirectionSet(directions));
}

bool DirectionSet::DoExistAll(DirectionSet directions) const {
	return (buffer & directions.buffer) == directions.buffer;
}

bool DirectionSet::DoExistAll(const std::list<Direction> &directions) const {
	return DoExistAll(DirectionSet(directions));
}

DirectionSet DirectionSet::Rotate(Direction direction) const {
	size_t shift;
	switch (direction) {
		case Direction::WEST:
			shift = 1;
			break;
		case Direction::NORTH:
			shift = 2;
			break;
		case Direction::EAST:
			shift = 3;
			break;
		default:
			return *this;
	}

	DirectionSet result = *this;

	for (size_t i = 0; i < 3; i++)
		result.buffer[(i + shift) % 4] = buffer[i];

	return result;
}

void DirectionSet::Reset() {
	buffer.reset();
}

const DirectionSet::BufferType &DirectionSet::GetBuffer() const { return buffer; }
void DirectionSet::SetBuffer(DirectionSet::BufferType buffer) { this->buffer = buffer; }




DirectionSetFractional::DirectionSetFractional(std::initializer_list<DirectionFractional> fractDirections) {
	Add(fractDirections);
}

void DirectionSetFractional::Add(std::initializer_list<DirectionFractional> fractDirections) {
	for (auto fractDirection : fractDirections) {
		fractions[index(fractDirection.direction)] = fractDirection.fraction;
	}
}

void DirectionSetFractional::Remove(std::initializer_list<Direction> directions) {
	for (auto direction : directions) {
		fractions[index(direction)] = 0.f;
	}
}

double DirectionSetFractional::GetMaxFraction(std::initializer_list<Direction> directions) const {
	float max = 0.f;
	for (auto direction : directions) {
		if (max < fractions[index(direction)])
			max = fractions[index(direction)];
	}
	return max;
}

void DirectionSetFractional::Reset() {
	std::fill(fractions.begin(), fractions.end(), 0.f);
}

const std::array<float, 5> &DirectionSetFractional::GetFractions() const { return fractions; }
void DirectionSetFractional::SetFractions(std::array<float, 5> &&fractions) { this->fractions = std::forward<std::array<float, 5>>(fractions); }

}
