#pragma once

#include "Detail/AngleConverters.h"

namespace uf {

class Angle {
public:
	static Angle Radians(double radians) { return Angle(radians); }
	static Angle Degrees(double degrees) { return Angle(uf::detail::DegressToRadians(degrees)); }

	double GetRadians() const { return radians; }
	double GetDegrees() const { return uf::detail::RadiansToDegrees(radians); }

	double Cos() const { return std::cos(radians); }
	double Sin() const { return std::sin(radians); }

	Angle operator-() { return Angle::Radians(-radians); }

	Angle operator+(const Angle &other) { return Angle::Radians(radians + other.radians); }
	Angle operator-(const Angle &other) { return Angle::Radians(radians - other.radians); }

	Angle operator+=(const Angle &other) { radians += other.radians; return *this; }
	Angle operator-=(const Angle &other) { radians -= other.radians; return *this; }

	bool operator==(const Angle &other) const { return radians == other.radians; }
	bool operator!=(const Angle &other) const { return radians != other.radians; }
	bool operator<=(const Angle &other) const { return radians <= other.radians; }
	bool operator<(const Angle &other) const { return radians < other.radians; }
	bool operator>=(const Angle &other) const { return radians >= other.radians; }
	bool operator>(const Angle &other) const { return radians > other.radians; }

private:
	explicit Angle(double radians) : 
		radians(radians) 
	{ }

	double radians;
};

}
