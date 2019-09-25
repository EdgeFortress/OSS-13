
namespace uf {

template<typename T>
void Grid<T>::SetSize(vec3u size) {
	dataSize = size;
	data.resize(size.x*size.y*size.z);
}

template<typename T>
void Grid<T>::SetSize(uint x, uint y, uint z) {
	SetSize({x,y,z});
}

template<typename T>
vec3u Grid<T>::GetSize() const {
	return dataSize;
}

template<typename T>
std::vector<T>& Grid<T>::Get() {
	return data;
}

template<typename T>
const std::vector<T>& Grid<T>::Get() const {
	return data;
}

template<typename T>
typename std::vector<T>::reference Grid<T>::At(vec3u pos) {
	return data[flatIndex(pos, dataSize.x, dataSize.y)];
}

template<typename T>
typename std::vector<T>::reference Grid<T>::At(uint x, uint y, uint z) {
	return At({x,y,z});
}

template<typename T>
typename std::vector<T>::const_reference Grid<T>::At(vec3u pos) const {
	return data[flatIndex(pos, dataSize.x, dataSize.y)];
}

template<typename T>
typename std::vector<T>::const_reference Grid<T>::At(uint x, uint y, uint z) const {
	return At({x,y,z});
}

template<typename T>
void Grid<T>::Transform(const GridTransformation transformation) {
	vec3i new_size = dataSize + transformation.sizeDelta;
	vec3i offset = transformation.originDelta;
	std::vector<T> new_data(new_size.x*new_size.y*new_size.z);
	for (uint x = 0; x < dataSize.x; x++)
	for (uint y = 0; y < dataSize.y; y++)
	for (uint z = 0; z < dataSize.z; z++) {
		vec3i new_pos = vec3u(x,y,z) - offset;
		if (new_pos >= vec3i(0) && new_pos < new_size) {
			new_data[flatIndex(new_pos, new_size.x, new_size.y)] = At(x,y,z);
			movedCallback({x,y,z}, new_pos);
		} else {
			removedCallback({x,y,z});
		}
	}
	dataSize = new_size;
	data = std::move(new_data);
}

template<typename T>
void Grid<T>::SetMovedCallback(std::function<void(vec3u, vec3u)> _movedCallback) {
	movedCallback = _movedCallback;
}

template<typename T>
void Grid<T>::SetRemovedCallback(std::function<void(vec3u)> _removedCallback) {
	removedCallback = _removedCallback;
}

template<typename T>
uint Grid<T>::flatIndex (const vec3u xyz, uint w, uint h) {
	return xyz.z * h * w + xyz.y * w + xyz.x;
}

} // namespace uf
