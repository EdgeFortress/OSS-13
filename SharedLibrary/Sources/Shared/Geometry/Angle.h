#pragma once

#include "Detail/AngleConverters.h"

namespace uf {

class Angle {
public:
	static Angle Radians(double radians) { return Angle(radians); }
	static Angle Degrees(double degrees) { return Angle(uf::detail::DegressToRadians(degrees)); }

	double GetRadians() { return radians; }
	double GetDegrees() { return uf::detail::RadiansToDegrees(radians); }

	double Cos() { return std::cos(radians); }
	double Sin() { return std::sin(radians); }

private:
	explicit Angle(double radians) : 
		radians(radians) 
	{ }

	double radians;
};

}
