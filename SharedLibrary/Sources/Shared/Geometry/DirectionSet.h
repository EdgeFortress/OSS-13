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
	DirectionSet() = default;
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


using DirectionFractional = std::tuple<Direction, float>;

class DirectionSetFractional {
	using BufferType = std::array<float, uf::DIRECTION_PURE_COUNT>;

public:
	DirectionSetFractional();
	DirectionSetFractional(const std::list<DirectionFractional> &fractDirections);

	void Set(DirectionFractional fractDirection);
	void Set(const std::list<DirectionFractional> &fractDirections);

	void Remove(Direction direction);
	void Remove(const std::list<Direction> &directions);

	float GetFraction(Direction direction) const;
	float GetCumulativeFraction(const std::list<Direction> &directions) const;
	bool IsDefault() const;

	DirectionSetFractional Rotate(Direction direction) const;

	void Reset();

	const BufferType &GetFractions() const;
	void SetFractions(BufferType fractions);

	bool operator==(const DirectionSetFractional &other) const;
	bool operator!=(const DirectionSetFractional &other) const;
	DirectionSetFractional operator+(const DirectionSetFractional &other) const;
	DirectionSetFractional operator+=(const DirectionSetFractional &other);
	DirectionSetFractional operator-(const DirectionSetFractional &other) const;
	DirectionSetFractional operator-=(const DirectionSetFractional &other);

private:
	BufferType fractions;
};

}
