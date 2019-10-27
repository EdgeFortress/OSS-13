#include "DirectionSet.h"

namespace uf {

namespace {

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

template<class BufferType>
BufferType rotate(const BufferType &buffer, Direction direction) {
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
			return buffer;
	}

	BufferType result = buffer;

	for (size_t i = 0; i < 4; i++)
		result[(i + shift) % 4] = buffer[i];

	return result;
}

} // namespace

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
	DirectionSet result;
	result.buffer = std::move(rotate<DirectionSet::BufferType>(buffer, direction));
	return result;
}

void DirectionSet::Reset() {
	buffer.reset();
}

const DirectionSet::BufferType &DirectionSet::GetBuffer() const { return buffer; }
void DirectionSet::SetBuffer(DirectionSet::BufferType buffer) { this->buffer = buffer; }


// DirectionSetFractional

DirectionSetFractional::DirectionSetFractional() {
	Reset();
}

DirectionSetFractional::DirectionSetFractional(const std::list<DirectionFractional> &fractDirections) {
	DirectionSetFractional();
	Set(fractDirections);
}

void DirectionSetFractional::Set(DirectionFractional fractDirection) {
	fractions[index(std::get<0>(fractDirection))] = std::get<1>(fractDirection);
}

void DirectionSetFractional::Set(const std::list<DirectionFractional> &fractDirections) {
	for (auto &fractDirection : fractDirections)
		Set(fractDirection);
}

void DirectionSetFractional::Remove(Direction direction) {
	fractions[index(direction)] = 1.f;
}

void DirectionSetFractional::Remove(const std::list<Direction> &directions) {
	for (auto &direction : directions)
		Remove(direction);
}

float DirectionSetFractional::GetFraction(Direction direction) const {
	return fractions.at(index(direction));
}

float DirectionSetFractional::GetCumulativeFraction(const std::list<Direction> &directions) const {
	float cumulative = 1.f;
	for (auto &direction : directions)
		cumulative *= GetFraction(direction);
	return cumulative;
}

bool DirectionSetFractional::IsDefault() const {
	return fractions == DirectionSetFractional().fractions;
}

DirectionSetFractional DirectionSetFractional::Rotate(Direction direction) const {
	DirectionSetFractional result;
	result.fractions = std::move(rotate<DirectionSetFractional::BufferType>(fractions, direction));
	return result;
}

void DirectionSetFractional::Reset() {
	fractions.fill(1.f);
}

const DirectionSetFractional::BufferType &DirectionSetFractional::GetFractions() const { return fractions; }
void DirectionSetFractional::SetFractions(DirectionSetFractional::BufferType fractions) { this->fractions = fractions; }

DirectionSetFractional DirectionSetFractional::operator+(const DirectionSetFractional &other) const {
	DirectionSetFractional result;
	for (size_t i = 0; i < fractions.size(); i++)
		result.fractions[i] = fractions[i] * other.fractions[i];
	return result;
}

DirectionSetFractional DirectionSetFractional::operator+=(const DirectionSetFractional &other) {
	for (size_t i = 0; i < fractions.size(); i++)
		fractions[i] *= other.fractions[i];
	return *this;
}

DirectionSetFractional DirectionSetFractional::operator-(const DirectionSetFractional &other) const {
	DirectionSetFractional result;
	for (size_t i = 0; i < fractions.size(); i++) {
		result.fractions[i] = fractions[i] / other.fractions[i];
		EXPECT(result.fractions[i] <= 1.f);
	}
	return result;
}

DirectionSetFractional DirectionSetFractional::operator-=(const DirectionSetFractional &other) {
	for (size_t i = 0; i < fractions.size(); i++) {
		fractions[i] /= other.fractions[i];
		EXPECT(fractions[i] <= 1.f);
	}
	return *this;
}

} // namespace uf
