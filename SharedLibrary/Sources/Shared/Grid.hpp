#pragma once

#include <functional>
#include <vector>

#include <Shared/Types.hpp>

namespace uf {

struct GridTransformation {
	vec3i originDelta = {0,0,0};
	vec3i sizeDelta = {0,0,0};
};

template<typename T>
class Grid {
public:
	Grid() {};
	void SetSize(vec3u size);
	void SetSize(uint x, uint y, uint z);
	vec3u GetSize() const;
	std::vector<T>& Get();
	const std::vector<T>& Get() const;
	void SetMovedCallback(std::function<void(vec3u, vec3u)> movedCallback);
	void SetRemovedCallback(std::function<void(vec3u)> removedCallback);
	void Transform(const GridTransformation transformation);
	typename std::vector<T>::reference At(vec3u pos);
	typename std::vector<T>::reference At(uint x, uint y, uint z);
	typename std::vector<T>::const_reference At(vec3u pos) const;
	typename std::vector<T>::const_reference At(uint x, uint y, uint z) const;
private:
	static uint flatIndex (const vec3u xyz, uint w, uint h);
	vec3u dataSize;
	std::vector<T> data;
	std::function<void(vec3u, vec3u)> movedCallback = [](vec3u, vec3u){};
	std::function<void(vec3u)> removedCallback = [](vec3u){};
};

} //namespace uf

#include "Grid.inl"
