#pragma once

#include <initializer_list>
#include <bitset>
#include <array>
#include <list>

#include <Shared/ErrorHandling.h>
#include <Shared/IFaces/ICopyable.h>
#include <Shared/Geometry/Direction.hpp>

namespace uf {

class DirectionSet : public ICopyable {
	using BufferType = std::bitset<uf::DIRECTION_PURE_COUNT>;

public:
	DirectionSet() { };
	DirectionSet(std::list<Direction> directions);

	void Add(DirectionSet directions);
	void Add(const std::list<Direction> &directions);

	void Remove(DirectionSet directions);
	void Remove(const std::list<Direction> &directions);

	bool DoesExistOne(DirectionSet directions) const;
	bool DoesExistOne(const std::list<Direction> &directions) const;

	bool DoExistAll(DirectionSet directions) const;
	bool DoExistAll(const std::list<Direction> &directions) const;

	DirectionSet Rotate(Direction direction) const;

	void Reset();

	const BufferType &GetBuffer() const;
	void SetBuffer(BufferType buffer);

private:
	BufferType buffer;
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
