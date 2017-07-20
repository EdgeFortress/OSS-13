#include "Vec2f.hpp"

#include <utility>
#include <SFML/System/Vector2.hpp>

using namespace uf;

vec2f::vec2f() : x(0), y(0) {}
vec2f::vec2f(float x, float y) : x(x), y(y) {}
vec2f::vec2f(const vec2f &vect) : x(vect.x), y(vect.y) { }
vec2f::vec2f(const sf::Vector2f &vect) : x(vect.x), y(vect.y) {}
vec2f::vec2f(const sf::Vector2u &vect) : x(float(vect.x)), y(float(vect.y)) {}
vec2f::vec2f(const sf::Vector2i &vect) : x(float(vect.x)), y(float(vect.y)) {}

float vec2f::length() const {
	return ::length(*this);
}

vec2f vec2f::normalize() const {
	return ::normalize(*this);
}

float vec2f::GetAngle() const {
	if (x == 0 && y == 0) return 0;
	if (x == 0) {
		if (y > 0) return float(M_PI / 2);
		else       return float(M_PI * 3 / 2);
	}
	float angle = 0;
	if (x > 0) angle = std::atan(y / x);
	if (x < 0) angle = float(M_PI - std::atan(y / -x));
	return angle;
}

vec2f vec2f::operator+(const vec2f &other) {
	return vec2f(x + other.x, y + other.y);
}
vec2f vec2f::operator-(const vec2f &other) {
	return vec2f(x - other.x, y - other.y);
}
vec2f vec2f::operator+=(const vec2f &other) {
	x += other.x;
	y += other.y;
	return std::move(*this);
}
vec2f vec2f::operator-=(const vec2f &other) {
	x -= other.x;
	y -= other.y;
	return std::move(*this);
}
vec2f vec2f::operator*(float a) const {
	return vec2f(x * a, y * a);
}
vec2f vec2f::operator*=(float a) {
	x *= a;
	y *= a;
	return std::move(*this);
}
vec2f vec2f::operator/(float a) const {
	return vec2f(x / a, y / a);
}
vec2f vec2f::operator/=(float a) {
	x /= a;
	y /= a;
	return std::move(*this);
}

vec2f::operator sf::Vector2f() const {
	return sf::Vector2f(x, y);
}
vec2f::operator sf::Vector2u() const {
	return sf::Vector2u((unsigned)x, (unsigned)y);
}
vec2f::operator sf::Vector2i() const {
	return sf::Vector2i((int)x, (int)y);
}

namespace uf {
	std::ostream &operator<<(std::ostream &os, const uf::vec2f &vec) {
		os << "(" << vec.x << ", " << vec.y << ')';
		return os;
	}
}

float uf::length(const vec2f &vec) {
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}
vec2f uf::normalize(const vec2f &vec) {
	return vec / length(vec);
}