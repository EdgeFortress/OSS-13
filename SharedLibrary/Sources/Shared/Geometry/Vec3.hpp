#pragma once

#include <SFML/System/Vector3.hpp>
#include <iostream>

namespace uf {
    template<typename T>
	struct vec3 {
		T x;
		T y;
		T z;

		vec3();
		vec3(T x, T y, T z);
		vec3(vec2<T> xy, T z);
		explicit vec3(T xyz);

        // implicit
		template<typename U>
		vec3(const vec3<U> &vect);
		template<typename U>
		vec3(const sf::Vector3<U> &vect);

		vec2<T> xy() const;

		double length() const;

		vec3<double> normalize() const;

		vec3<T> operator+=(const vec3<T> &other);
		vec3<T> operator-=(const vec3<T> &other);
		vec3<T> operator*=(T a);
        vec3<T> operator/=(T a);
        vec3<T> operator%=(T a);

        // True if true for both coordinates
		bool operator<=(const vec3<T> &other) const;
		bool operator<(const vec3<T> &other) const;
		bool operator>=(const vec3<T> &other) const;
		bool operator>(const vec3<T> &other) const;

        template<typename U>
		operator sf::Vector3<U>() const;
        // True if one of the coordinates is different from zero
        operator bool() const;

		std::string toString() { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"; }

        //friend std::ostream &operator<< <T>(std::ostream &, const vec3<T> &);
	};

	template<typename T>
	double length(const vec3<T> &vec);
	template<typename T>
	vec3<double> normalize(const vec3<T> &vec);

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
    //template<typename T>
    //std::ostream& operator<<(std::ostream&, const vec3<T> &);
}

#include "Vec3.inl"

namespace uf {
	typedef vec3<double> vec3d;
	typedef vec3<float>  vec3f;
	typedef vec3<int>    vec3i;
	typedef vec3<uint>   vec3u;
}