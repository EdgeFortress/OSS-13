#pragma once

#include <chrono>
#include <array>
#include <bitset>

#include <Shared/Network/Archive.h>
#include <Shared/ErrorHandling.h>
#include <Shared/Geometry/DirectionSet.h>

uf::Archive &operator&(uf::Archive &ar, long int &li);

template<class T>
uf::Archive &operator&(uf::Archive &ar, std::vector<T> &vector) {
	if (ar.IsOutput()) {
		EXPECT_WITH_MSG(vector.size() == 0, "Try unpack Archive to non-empty vector");
		sf::Int32 size;
		ar >> size;
		vector.resize(size);
	} else {
		ar << sf::Int32(vector.size());
	}
	for (auto &item : vector) {
		ar & item;
	}
	return ar;
}

template<size_t size>
uf::Archive &operator&(uf::Archive &ar, std::bitset<size> &set) {
	if (ar.IsOutput()) {
		sf::Int32 buffer;
		ar >> buffer;
		set = unsigned(buffer);
	} else {
		ar << sf::Int32(set.to_ulong());
	}
	return ar;
}

template<class T, size_t size>
uf::Archive &operator&(uf::Archive &ar, std::array<T, size> &array) {
	for (auto &item : array) {
		ar & item;
	}
	return ar;
}

uf::Archive &operator&(uf::Archive &ar, uf::Direction &d);
uf::Archive &operator&(uf::Archive &ar, uf::DirectionSet &directionSet);
uf::Archive &operator&(uf::Archive &ar, uf::DirectionSetFractional &directionSetFractional);

template<class T>
uf::Archive &operator&(uf::Archive &ar, uf::vec2<T> &vec) {
	ar & vec.x;
	ar & vec.y;
	return ar;
}

template<class T>
uf::Archive &operator&(uf::Archive &ar, uf::vec3<T> &vec) {
	ar & vec.x;
	ar & vec.y;
	ar & vec.z;
	return ar;
}

template<class RepType, class Ratio>
uf::Archive &operator&(uf::Archive &ar, std::chrono::duration<RepType, Ratio> &duration) {
	if (ar.IsOutput()) {
		RepType buf;
		ar >> buf;
		duration = std::chrono::duration<RepType, Ratio>(buf);
	} else {
		ar << duration.count();
	}
	return ar;
}

