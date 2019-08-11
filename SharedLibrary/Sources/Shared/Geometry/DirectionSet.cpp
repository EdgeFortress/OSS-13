#include "DirectionSet.h"

namespace uf {

constexpr size_t directionToIndex(Direction direction) {
	if (direction == Direction::CENTER)
		return 4;
	EXPECT(char(direction) < 5);
	return char(direction);
}

DirectionSet::DirectionSet(std::initializer_list<Direction> directions) {
	Add(directions);
}

void DirectionSet::Add(DirectionSet directions) {
	buffer |= directions.buffer;
}

void DirectionSet::Add(std::initializer_list<Direction> directions) {
	for (auto direction : directions) {
		Direction first, second;
		if (SplitDirection(direction, first, second)) {
			buffer[directionToIndex(first)] = true;
			buffer[directionToIndex(second)] = true;
		} else {
			buffer[directionToIndex(direction)] = true;
		}
	}
}

void DirectionSet::Remove(DirectionSet directions) {
	buffer &= ~directions.buffer;
}

void DirectionSet::Remove(std::initializer_list<Direction> directions) {
	Remove(DirectionSet(directions));
}

bool DirectionSet::IsExistsOne(DirectionSet directions) const {
	return (buffer & directions.buffer).any();
}

bool DirectionSet::IsExistsOne(std::initializer_list<Direction> directions) const {
	return IsExistsOne(DirectionSet(directions));
}

bool DirectionSet::AreExistAll(DirectionSet directions) const {
	return (buffer & directions.buffer) == directions.buffer;
}

bool DirectionSet::AreExistAll(std::initializer_list<Direction> directions) const {
	return AreExistAll(DirectionSet(directions));
}

void DirectionSet::Reset() {
	buffer.reset();
}

const std::bitset<5> &DirectionSet::GetBuffer() const { return buffer; }
void DirectionSet::SetBuffer(std::bitset<5> buffer) { this->buffer = buffer; }




DirectionSetFractional::DirectionSetFractional(std::initializer_list<DirectionFractional> fractDirections) {
	Add(fractDirections);
}

void DirectionSetFractional::Add(std::initializer_list<DirectionFractional> fractDirections) {
	for (auto fractDirection : fractDirections) {
		fractions[directionToIndex(fractDirection.direction)] = fractDirection.fraction;
	}
}

void DirectionSetFractional::Remove(std::initializer_list<Direction> directions) {
	for (auto direction : directions) {
		fractions[directionToIndex(direction)] = 0.f;
	}
}

double DirectionSetFractional::GetMaxFraction(std::initializer_list<Direction> directions) const {
	float max = 0.f;
	for (auto direction : directions) {
		if (max < fractions[directionToIndex(direction)])
			max = fractions[directionToIndex(direction)];
	}
	return max;
}

void DirectionSetFractional::Reset() {
	std::fill(fractions.begin(), fractions.end(), 0.f);
}

const std::array<float, 5> &DirectionSetFractional::GetFractions() const { return fractions; }
void DirectionSetFractional::SetFractions(std::array<float, 5> &&fractions) { this->fractions = std::forward<std::array<float, 5>>(fractions); }

}
