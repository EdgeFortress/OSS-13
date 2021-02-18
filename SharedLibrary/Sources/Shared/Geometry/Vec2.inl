#pragma once

#include <cmath>

namespace uf {

template <typename T>
vec2<T>::vec2() : x(0), y(0) {}

template <typename T>
vec2<T>::vec2(T x, T y) : x(x), y(y) {}

template <typename T>
vec2<T>::vec2(T xy) : x(xy), y(xy) { }

template <typename T>
auto vec2<T>::unit(Angle angle) {
	std::enable_if_t<std::is_floating_point<T>::value, vec2<T>> result;
	result.x = static_cast<T>(angle.Cos());
	result.y = static_cast<T>(angle.Sin());
	return result;
}

template <typename T>
vec2<double> vec2<T>::unit() const {
	return *this / magnitude();
}

template <typename T>
vec2<double> vec2<T>::rotate(Angle angle) const {
	if (isZero()) 
		return {};
	return vec2<double>::unit(angle + this->angle()) * magnitude();
}

template <typename T>
double vec2<T>::magnitude() const {
	return sqrt(x * x + y * y);
}

template <typename T>
bool vec2<T>::isZero() const {
	return x == T{} && y == T{};
}

template <typename T>
Angle vec2<T>::angle() const {
	static_assert(std::is_signed_v<T>);
	return Angle::Radians(std::atan2(y, x));
}

template <typename T>
vec2<T> vec2<T>::operator+=(const vec2<T>& other) {
	x += other.x;
	y += other.y;
	return std::move(*this);
}

template <typename T>
vec2<T> vec2<T>::operator-=(const vec2<T>& other) {
	x -= other.x;
	y -= other.y;
	return std::move(*this);
}

template <typename T>
vec2<T> vec2<T>::operator*=(T a) {
	x *= a;
	y *= a;
	return std::move(*this);
}

template <typename T>
vec2<T> vec2<T>::operator/=(T a) {
	x /= a;
	y /= a;
	return std::move(*this);
}

template <typename T>
vec2<T> vec2<T>::operator%=(T a) {
	x %= a;
	y %= a;
	return std::move(*this);
}

template <typename T>
bool vec2<T>::operator==(const vec2<T>& other) const {
	return x == other.x && y == other.y;
}

template <typename T>
bool vec2<T>::operator!=(const vec2<T>& other) const {
	return x != other.x || y != other.y;
}

template <typename T>
bool vec2<T>::operator<=(const vec2<T>& other) const {
	return x <= other.x && y <= other.y;
}

template <typename T>
bool vec2<T>::operator<(const vec2<T>& other) const {
	return x < other.x && y < other.y;
}

template <typename T>
bool vec2<T>::operator>=(const vec2<T>& other) const {
	return x >= other.x && y >= other.y;
}

template <typename T>
bool vec2<T>::operator>(const vec2<T>& other) const {
	return x > other.x && y > other.y;
}

template <typename T>
template <typename U>
vec2<T>::vec2(const vec2<U>& vect) : x(T(vect.x)), y(T(vect.y)) {}

template <typename T>
template <typename U>
vec2<T>::vec2(const sf::Vector2<U>& vect) : x(T(vect.x)), y(T(vect.y)) {}

template <typename T>
template <typename U>
vec2<T>::operator sf::Vector2<U>() const {
	return sf::Vector2<U>(U(x), U(y));
}

template <typename T>
auto operator-(const vec2<T>& vec) {
	if constexpr (std::is_unsigned<T>::value)
		return uf::vec2<std::make_signed<T>>(vec) * -1;
	else
		return vec * -1;
}

template <typename LeftT, typename RightT>
auto operator+(const vec2<LeftT>& left, const vec2<RightT>& right) {
	auto x = left.x + right.x;
	return uf::vec2<decltype(x)>(x, left.y + right.y);
}

template <typename LeftT, typename RightT>
auto operator-(const vec2<LeftT>& left, const vec2<RightT>& right) {
	auto x = left.x - right.x;
	return uf::vec2<decltype(x)>(x, left.y - right.y);
}

template <typename VectorT, typename ScalarT>
auto operator*(const vec2<VectorT> &vector, const ScalarT &scalar) {
	auto x = vector.x * scalar;
	return uf::vec2<decltype(x)>(x, vector.y * scalar);
}
template <typename VectorT, typename ScalarT>
auto operator/(const vec2<VectorT> &vector, const ScalarT &scalar) {
	auto x = vector.x / scalar;
	return uf::vec2<decltype(x)>(x, vector.y / scalar);
}
template <typename VectorT, typename ScalarT>
auto operator%(const vec2<VectorT> &vector, const ScalarT &scalar) {
	auto x = vector.x % scalar;
	return uf::vec2<decltype(x)>(x, vector.y % scalar);
}

} // namespace uf
