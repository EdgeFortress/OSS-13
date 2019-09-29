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

inline auto operator-(const Angle& angle) { return Angle::Radians(-angle.GetRadians()); }

inline auto operator+(const Angle& left, const Angle& right) { return Angle::Radians(left.GetRadians() + right.GetRadians()); }
inline auto operator-(const Angle& left, const Angle& right) { return Angle::Radians(left.GetRadians() - right.GetRadians()); }

}
