#pragma once

#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace uf {
    template<typename T>
	struct vec2 {
		T x;
		T y;

		vec2();
		vec2(T x, T y);
		explicit vec2(T xy);

        // implicit
		template<typename U>
		vec2(const vec2<U> &vect);
		template<typename U>
		vec2(const sf::Vector2<U> &vect);

		double length() const;

		vec2<float> normalize() const;

		// Angle in radians counter-clockwise ((1, 0) - 0 angle)
		double GetAngle() const;

		vec2<T> operator+=(const vec2<T> &other);
		vec2<T> operator-=(const vec2<T> &other);
		vec2<T> operator*=(T a);
        vec2<T> operator/=(T a);
        vec2<T> operator%=(T a);

        // True if true for both coordinates
		bool operator<=(const vec2<T> &other) const;
        // True if true for both coordinates
		bool operator<(const vec2<T> &other) const;
        // True if true for both coordinates
		bool operator>=(const vec2<T> &other) const;
        // True if true for both coordinates
		bool operator>(const vec2<T> &other) const;

        template<typename U>
		operator sf::Vector2<U>() const;
        // True if one of the coordinates is different from zero
        operator bool() const;

		std::string toString() { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

        //friend std::ostream &operator<< <T>(std::ostream &, const vec2<T> &);
	};

	template<typename T>
	double length(const vec2<T> &vec);
	template<typename T>
	vec2<double> normalize(const vec2<T> &vec);

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
    //template<typename T>
    //std::ostream& operator<<(std::ostream&, const vec2<T> &);
}

#include "Vec2.inl"

namespace uf {
	typedef vec2<double>     vec2d;
	typedef vec2<float>      vec2f;
	typedef vec2<int>        vec2i;
	typedef vec2<uint32_t>   vec2u;
}