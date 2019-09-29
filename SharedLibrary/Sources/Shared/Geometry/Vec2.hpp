#pragma once

#include <iostream>

#include <SFML/System/Vector2.hpp>

#include "Angle.h"

namespace uf {

template<
	typename T, 
	typename = typename std::enable_if_t<std::is_arithmetic_v<T>>
>
struct vec2;

template<typename T>
struct vec2<T> {
	T x;
	T y;

	vec2();
	vec2(T x, T y);
	explicit vec2(T xy);

	static auto unit(Angle angle);

	double magnitude() const;
	bool isZero() const;

	vec2<double> unit() const;
	vec2<double> rotate(Angle angle) const;

	// Angle in radians counter-clockwise ((1, 0) - 0 angle)
	Angle angle() const;

	vec2<T> operator+=(const vec2<T> &other);
	vec2<T> operator-=(const vec2<T> &other);
	vec2<T> operator*=(T a);
	vec2<T> operator/=(T a);
	vec2<T> operator%=(T a);

	// True if true for both coordinates
	bool operator==(const vec2<T> &other) const;
	// True if true for both coordinates
	bool operator!=(const vec2<T> &other) const;
	// True if true for both coordinates
	bool operator<=(const vec2<T> &other) const;
	// True if true for both coordinates
	bool operator<(const vec2<T> &other) const;
	// True if true for both coordinates
	bool operator>=(const vec2<T> &other) const;
	// True if true for both coordinates
	bool operator>(const vec2<T> &other) const;

	// implicit converts
	template<typename U>
	vec2(const vec2<U> &vect);
	template<typename U>
	vec2(const sf::Vector2<U> &vect);
	template<typename U>
	operator sf::Vector2<U>() const;

	std::string toString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }
};

template<typename T>
auto operator-(const vec2<T>& vec);

template <typename LeftT, typename RightT>
auto operator+(const vec2<LeftT>& left, const vec2<RightT>& right);
template <typename LeftT, typename RightT>
auto operator-(const vec2<LeftT>& left, const vec2<RightT>& right);
template <typename VectorT, typename ScalarT>
auto operator*(const vec2<VectorT> &, const ScalarT &);
template <typename VectorT, typename ScalarT>
auto operator/(const vec2<VectorT> &, const ScalarT &);
template <typename VectorT, typename ScalarT>
auto operator%(const vec2<VectorT> &, const ScalarT &);

} // namespace uf

#include "Vec2.inl"

namespace uf {

typedef vec2<double>     vec2d;
typedef vec2<float>      vec2f;
typedef vec2<int>        vec2i;
typedef vec2<uint32_t>   vec2u;

} // namespace uf
