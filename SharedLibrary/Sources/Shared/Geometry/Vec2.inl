#include <cmath>

namespace uf {

template<typename T>
    vec2<T>::vec2() : x(0), y(0) {}

template<typename T>
    vec2<T>::vec2(T x, T y) : x(x), y(y) {}

template<typename T>
vec2<T>::vec2(T xy) : x(xy), y(xy) { }

template<typename T>
template<typename U>
    vec2<T>::vec2(const vec2 <U> &vect) : x(T(vect.x)), y(T(vect.y)) {}

template<typename T>
template<typename U>
    vec2<T>::vec2(const sf::Vector2 <U> &vect) : x(T(vect.x)), y(T(vect.y)) {}

template<typename T>
double vec2<T>::length() const {
        return uf::length<T>(*this);
}

template<typename T>
vec2<double> vec2<T>::normalize() const {
        return uf::normalize<T>(*this);
}

template<typename T>
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

template<typename T>
    vec2 <T> vec2<T>::operator+(const vec2 <T> &other) {
	return vec2<T>(x + other.x, y + other.y);
}

template<typename T>
    vec2 <T> vec2<T>::operator-(const vec2 <T> &other) {
	return vec2<T>(x - other.x, y - other.y);
}

template<typename T>
    vec2 <T> vec2<T>::operator+=(const vec2 <T> &other) {
	x += other.x;
	y += other.y;
	return std::move(*this);
}

template<typename T>
    vec2 <T> vec2<T>::operator-=(const vec2 <T> &other) {
	x -= other.x;
	y -= other.y;
	return std::move(*this);
}

template<typename T>
    vec2 <T> vec2<T>::operator*(T a) const {
	return vec2<T>(x * a, y * a);
}

template<typename T>
    vec2 <T> vec2<T>::operator*=(T a) {
	x *= a;
	y *= a;
	return std::move(*this);
}

template<typename T>
    vec2 <T> vec2<T>::operator/(double a) const {
	return vec2<T>(x / a, y / a);
}

template<typename T>
    vec2 <T> vec2<T>::operator/=(double a) {
	x /= a;
	y /= a;
	return std::move(*this);
}

template<typename T>
bool vec2<T>::operator<=(const vec2<T> &other) const {
	return x <= other.x && y <= other.y;
}
template<typename T>
bool vec2<T>::operator<(const vec2<T> &other) const {
	return x < other.x && y < other.y;
}
template<typename T>
bool vec2<T>::operator>=(const vec2<T> &other) const {
	return x >= other.x && y >= other.y;
}
template<typename T>
bool vec2<T>::operator>(const vec2<T> &other) const {
	return x > other.x && y > other.y;
}

template<typename T>
vec2<T>::operator sf::Vector2<float>() const {
	return sf::Vector2<float>(float(x), float(y));
}

template<typename T>
vec2<T>::operator sf::Vector2<uint>() const {
	return sf::Vector2<uint>(uint(x), uint(y));
}

template<typename T>
vec2<T>::operator sf::Vector2<int>() const {
	return sf::Vector2<int>(int(x), int(y));
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const uf::vec2<T> &vec) {
	os << "(" << vec.x << ", " << vec.y << ')';
	return os;
}

template<typename T>
    double length(const vec2 <T> &vec) {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
}

template<typename T>
    vec2<double> normalize(const vec2 <T> &vec) {
	return vec / length(vec);
}
}