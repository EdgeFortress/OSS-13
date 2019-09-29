#pragma once

#include <SFML/System/Vector3.hpp>
#include <iostream>

namespace uf {

template<
	typename T,
	typename = typename std::enable_if_t<std::is_arithmetic_v<T>>
>
struct vec3;

template<typename T>
struct vec3<T> {
	T x;
	T y;
	T z;

	vec3();
	vec3(T x, T y, T z);
	vec3(vec2<T> xy, T z);
	explicit vec3(T xyz);

	double magnitude() const;
	bool isZero() const;

	vec3<double> unit() const;
	vec2<T> xy() const;

	vec3<T> operator+=(const vec3<T> &other);
	vec3<T> operator-=(const vec3<T> &other);
	vec3<T> operator*=(T a);
	vec3<T> operator/=(T a);
	vec3<T> operator%=(T a);

	// True if true for both coordinates
	bool operator==(const vec3<T> &other) const;
	// True if true for both coordinates
	bool operator!=(const vec3<T> &other) const;
	// True if true for both coordinates
	bool operator<=(const vec3<T> &other) const;
	// True if true for both coordinates
	bool operator<(const vec3<T> &other) const;
	// True if true for both coordinates
	bool operator>=(const vec3<T> &other) const;
	// True if true for both coordinates
	bool operator>(const vec3<T> &other) const;

	// implicit
	template<typename U>
	vec3(const vec3<U> &vect);
	template<typename U>
	vec3(const sf::Vector3<U> &vect);
	template<typename U>
	operator sf::Vector3<U>() const;

	std::string toString() { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"; }
};

template<typename T>
auto operator-(const vec3<T>& vec);

template <typename LeftT, typename RightT>
auto operator+(const vec3<LeftT>& left, const vec3<RightT>& right);
template <typename LeftT, typename RightT>
auto operator-(const vec3<LeftT>& left, const vec3<RightT>& right);
template <typename VectorT, typename ScalarT>
auto operator*(const vec3<VectorT> &, const ScalarT &);
template <typename VectorT, typename ScalarT>
auto operator/(const vec3<VectorT> &, const ScalarT &);
template <typename VectorT, typename ScalarT>
auto operator%(const vec3<VectorT> &, const ScalarT &);

} // namespace uf

#include "Vec3.inl"

namespace uf {

typedef vec3<double>     vec3d;
typedef vec3<float>      vec3f;
typedef vec3<int>        vec3i;
typedef vec3<uint32_t>   vec3u;

} // namespace uf
