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
    template <typename U>
    vec2<T>::vec2(const vec2<U>& vect) : x(T(vect.x)), y(T(vect.y)) {}

    template <typename T>
    template <typename U>
    vec2<T>::vec2(const sf::Vector2<U>& vect) : x(T(vect.x)), y(T(vect.y)) {}

    template <typename T>
    double vec2<T>::length() const {
        return uf::length<T>(*this);
    }

    template <typename T>
    vec2<float> vec2<T>::normalize() const {
        return uf::normalize<T>(*this);
    }

    template <typename T>
    double vec2<T>::GetAngle() const {
        if (x == 0 && y == 0) return 0;
        if (x == 0) {
            if (y > 0) return M_PI / 2;
            else return M_PI * 3 / 2;
        }
        float angle = 0;
        if (x > 0) angle = atan(y / x);
        if (x < 0) angle = M_PI - atan(y / -x);
        return angle;
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
    vec2<T>::operator sf::Vector2<U>() const {
        return sf::Vector2<U>(U(x), U(y));
    }

    template <typename T>
    vec2<T>::operator bool() const {
        return x != 0 || y != 0;
    }

    //template <typename T>
    //std::ostream& operator<<(std::ostream& os, const uf::vec2<T>& vec) {
    //    os << "(" << vec.x << ", " << vec.y << ')';
    //    return os;
    //}

    template <typename T>
    double length(const vec2<T>& vec) {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    template <typename T>
    vec2<double> normalize(const vec2<T>& vec) {
        return vec / length(vec);
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
}
