#pragma once

namespace uf {

template <typename T>
vec3<T>::vec3() : x(0), y(0), z(0) {}

template <typename T>
vec3<T>::vec3(T x, T y, T z) : x(x), y(y), z(z) {}

template <typename T>
vec3<T>::vec3(T xyz) : x(xyz), y(xyz), z(xyz) { }

template <typename T>
vec3<T>::vec3(vec2<T> xy, T z) : x(xy.x), y(xy.y), z(z) { }

template <typename T>
vec3<double> vec3<T>::unit() const {
	return *this / magnitude();
}

template <typename T>
double vec3<T>::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

template <typename T>
bool vec3<T>::isZero() const {
	return x == T{} && y == T{} && z == T{};
}

template <typename T>
vec2<T> vec3<T>::xy() const {
	return {x, y};
}

template <typename T>
vec3<T> vec3<T>::operator+=(const vec3<T>& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return std::move(*this);
}

template <typename T>
vec3<T> vec3<T>::operator-=(const vec3<T>& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return std::move(*this);
}

template <typename T>
vec3<T> vec3<T>::operator*=(T a) {
	x *= a;
	y *= a;
	z *= a;
	return std::move(*this);
}

template <typename T>
vec3<T> vec3<T>::operator/=(T a) {
	x /= a;
	y /= a;
	z /= a;
	return std::move(*this);
}

template <typename T>
vec3<T> vec3<T>::operator%=(T a) {
	x %= a;
	y %= a;
	z %= a;
	return std::move(*this);
}

template <typename T>
bool vec3<T>::operator==(const vec3<T>& other) const {
	return x == other.x && y == other.y && z == other.z;
}

template <typename T>
bool vec3<T>::operator!=(const vec3<T>& other) const {
	return x != other.x || y != other.y || z != other.z;
}

template <typename T>
bool vec3<T>::operator<=(const vec3<T>& other) const {
	return x <= other.x && y <= other.y && z <= other.z;
}

template <typename T>
bool vec3<T>::operator<(const vec3<T>& other) const {
	return x < other.x && y < other.y && z < other.z;
}

template <typename T>
bool vec3<T>::operator>=(const vec3<T>& other) const {
	return x >= other.x && y >= other.y && z >= other.z;
}

template <typename T>
bool vec3<T>::operator>(const vec3<T>& other) const {
	return x > other.x && y > other.y && z > other.z;
}

template <typename T>
template <typename U>
vec3<T>::vec3(const vec3<U>& vect) : x(T(vect.x)), y(T(vect.y)), z(T(vect.z)) {}

template <typename T>
template <typename U>
vec3<T>::vec3(const sf::Vector3<U>& vect) : x(T(vect.x)), y(T(vect.y)), z(T(vect.z)) {}

template <typename T>
template <typename U>
vec3<T>::operator sf::Vector3<U>() const {
	return sf::Vector3<U>(U(x), U(y), U(z));
}

template <typename T>
auto operator-(const vec3<T>& vec) {
	if constexpr (std::is_unsigned<T>::value)
		return uf::vec3<std::make_signed<T>>(vec) * -1;
	else
		return vec * -1;
}

template <typename LeftT, typename RightT>
auto operator+(const vec3<LeftT>& left, const vec3<RightT>& right) {
	auto x = left.x + right.x;
	auto y = left.y + right.y;
	return uf::vec3<decltype(x)>(x, y, left.z + right.z);
}

template <typename LeftT, typename RightT>
auto operator-(const vec3<LeftT>& left, const vec3<RightT>& right) {
	auto x = left.x - right.x;
	auto y = left.y - right.y;
	return uf::vec3<decltype(x)>(x, y, left.z - right.z);
}

template <typename VectorT, typename ScalarT>\
auto operator*(const vec3<VectorT> &vector, const ScalarT &scalar) {
	auto x = vector.x * scalar;
	auto y = vector.y * scalar;
	return uf::vec3<decltype(x)>(x, y, vector.z * scalar);
}
template <typename VectorT, typename ScalarT>
auto operator/(const vec3<VectorT> &vector, const ScalarT &scalar) {
	auto x = vector.x / scalar;
	auto y = vector.y / scalar;
	return uf::vec3<decltype(x)>(x, y, vector.z / scalar);
}
template <typename VectorT, typename ScalarT>
auto operator%(const vec3<VectorT> &vector, const ScalarT &scalar) {
	auto x = vector.x % scalar;
	auto y = vector.y % scalar;
	return uf::vec3<decltype(x)>(x, y, vector.z % scalar);
}

} // namespace uf
