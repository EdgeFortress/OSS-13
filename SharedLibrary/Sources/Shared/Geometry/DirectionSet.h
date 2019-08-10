#pragma once

#include <initializer_list>
#include <array>

#include <Shared/ErrorHandling.h>
#include <Shared/Geometry/Direction.hpp>

namespace uf {

class DirectionSet {
public:
	DirectionSet(std::initializer_list<Direction> directions = {}) {
		buffer = 0;
		Add(directions);
	}

	DirectionSet(const DirectionSet &) = default;
	DirectionSet(DirectionSet &&) = default;
	DirectionSet &operator=(const DirectionSet &) = default;
	DirectionSet &operator=(DirectionSet &&) = default;

	void Add(std::initializer_list<Direction> directions) {
		for (auto direction : directions) {
			buffer |= static_cast<char>(direction);
		}
	}

	void Remove(std::initializer_list<Direction> directions) {
		for (auto direction : directions) {
			buffer &= ~static_cast<char>(direction);
		}
	}

	bool IsExistsOne(std::initializer_list<Direction> directions) const {
		for (auto direction : directions) {
			if (buffer & static_cast<char>(direction))
				return true;
		}
		return false;
	}

	bool AreExistAll(std::initializer_list<Direction> directions) const {
		for (auto direction : directions) {
			if (!(buffer & static_cast<char>(direction)))
				return false;
		}
		return true;
	}

	void Reset() {
		buffer = 0;
	}

	char GetBuffer() const { return buffer; }
	void SetBuffer(char buffer) { this->buffer = buffer; }

private:
	char buffer;
};

struct DirectionFractional {
	Direction direction;
	float fraction;
};

class DirectionSetFractional {
public:
	DirectionSetFractional(std::initializer_list<DirectionFractional> fractDirections = {}) {
		Add(fractDirections);
	}

	DirectionSetFractional(const DirectionSetFractional &) = default;
	DirectionSetFractional(DirectionSetFractional &&) = default;
	DirectionSetFractional &operator=(const DirectionSetFractional &) = default;
	DirectionSetFractional &operator=(DirectionSetFractional &&) = default;

	void Add(std::initializer_list<DirectionFractional> fractDirections) {
		for (auto fractDirection : fractDirections) {
			fractions[directionToIndex(fractDirection.direction)] = fractDirection.fraction;
		}
	}

	void Remove(std::initializer_list<Direction> directions) {
		for (auto direction : directions) {
			fractions[directionToIndex(direction)] = 0.f;
		}
	}

	double GetMaxFraction(std::initializer_list<Direction> directions) const {
		float max = 0.f;
		for (auto direction : directions) {
			if (max < fractions[directionToIndex(direction)])
				max = fractions[directionToIndex(direction)];
		}
		return max;
	}

	void Reset() {
		std::fill(fractions.begin(), fractions.end(), 0.f);
	}

	std::array<float, 5> GetFractions() const { return fractions; }
	void SetFractions(std::array<float, 5> &&fractions) { this->fractions = std::forward<std::array<float, 5>>(fractions); }

private:
	constexpr size_t directionToIndex(Direction direction) const {
		if (direction == Direction::CENTER)
			return 0;
		EXPECT(char(direction) < 5);
		return char(direction);
	}

	std::array<float, 5> fractions;
};

}
