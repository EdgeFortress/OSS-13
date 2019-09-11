#pragma once

#include <initializer_list>
#include <bitset>
#include <array>
#include <list>

#include <Shared/ErrorHandling.h>
#include <Shared/Geometry/Direction.hpp>

namespace uf {

class DirectionSet {
public:
	DirectionSet() = default;
	DirectionSet(std::list<Direction> directions);

	DirectionSet(const DirectionSet &) = default;
	DirectionSet(DirectionSet &&) = default;
	DirectionSet &operator=(const DirectionSet &) = default;
	DirectionSet &operator=(DirectionSet &&) = default;

	void Add(DirectionSet directions);
	void Add(const std::list<Direction> &directions);

	void Remove(DirectionSet directions);
	void Remove(const std::list<Direction> &directions);

	bool IsExistsOne(DirectionSet directions) const;
	bool IsExistsOne(const std::list<Direction> &directions) const;

	bool AreExistAll(DirectionSet directions) const;
	bool AreExistAll(const std::list<Direction> &directions) const;

	void Reset();

	const std::bitset<5> &GetBuffer() const;
	void SetBuffer(std::bitset<5> buffer);

private:
	std::bitset<5> buffer;
};

struct DirectionFractional {
	Direction direction;
	float fraction;
};


class DirectionSetFractional {
public:
	DirectionSetFractional() = default;
	DirectionSetFractional(std::initializer_list<DirectionFractional> fractDirections);

	DirectionSetFractional(const DirectionSetFractional &) = default;
	DirectionSetFractional(DirectionSetFractional &&) = default;
	DirectionSetFractional &operator=(const DirectionSetFractional &) = default;
	DirectionSetFractional &operator=(DirectionSetFractional &&) = default;

	void Add(std::initializer_list<DirectionFractional> fractDirections);
	void Remove(std::initializer_list<Direction> directions);
	double GetMaxFraction(std::initializer_list<Direction> directions) const;
	void Reset();

	const std::array<float, 5> &GetFractions() const;
	void SetFractions(std::array<float, 5> &&fractions);

private:
	std::array<float, 5> fractions;
};

}
