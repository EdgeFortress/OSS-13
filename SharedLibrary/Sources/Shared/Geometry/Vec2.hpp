#pragma once

#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "Types.hpp"

namespace uf {

	template<typename T>
	struct vec2 {
		T x;
		T y;

		vec2();
		vec2(T x, T y);
		template<typename U>
		vec2(const vec2<U> &vect);
		template<typename U>
		vec2(const sf::Vector2<U> &vect);

		double length() const;
		vec2<double> normalize() const;

		// Angle in radians counter-clockwise ((1, 0) - 0 angle)
		double GetAngle() const;

		vec2<T> operator+(const vec2<T> &other);
		vec2<T> operator-(const vec2<T> &other);
		vec2<T> operator+=(const vec2<T> &other);
		vec2<T> operator-=(const vec2<T> &other);
		vec2<T> operator*(T a) const;
		vec2<T> operator*=(T a);
		vec2<T> operator/(double a) const;
		vec2<T> operator/=(double a);

		operator sf::Vector2<float>() const;
		operator sf::Vector2<uint>() const;
		operator sf::Vector2<int>() const;

		friend std::ostream &operator<<(std::ostream &os, const vec2<T> &vec);
	};

	template<typename T>
	double length(const vec2<T> &vec);
	template<typename T>
	vec2<double> normalize(const vec2<T> &vec);

	#include "Vec2.inl"

	typedef vec2<double> vec2d;
	typedef vec2<float>  vec2f;
	typedef vec2<int>    vec2i;
	typedef vec2<uint>   vec2u;
}