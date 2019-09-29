#pragma once

#include <cmath>

namespace uf {
namespace detail {

const double PI = std::acos(-1);

inline double RadiansToDegrees(double radians) {
	return radians / PI * 180;
}

inline double DegressToRadians(double degrees) {
	return degrees / 180 * PI;
}

} // namespace detail
} // namespace uf
