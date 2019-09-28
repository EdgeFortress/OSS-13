namespace uf {

// Grid::iterator

template<typename T>
typename Grid<T>::iterator& Grid<T>::iterator::operator++() {
	vec3u dataSize = grid.GetSize();
	if(++dataPos.x == dataSize.x) {
		if(++dataPos.y == dataSize.y) {
			++dataPos.z;
			dataPos.y = 0;
		}
		dataPos.x = 0;
	}
	return *this;
}

template<typename T>
std::pair<typename Grid<T>::reference, vec3u> Grid<T>::iterator::operator*() {
	return {grid.At(dataPos), dataPos};
}

template<typename T>
bool Grid<T>::iterator::operator!=(const iterator& it) const {
	return it.dataPos != dataPos;
}

// Grid

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
typename Grid<T>::iterator Grid<T>::begin() {
	return iterator(*this);
}

template<typename T>
typename Grid<T>::iterator Grid<T>::end() {
	return iterator(*this, vec3u(0, 0, dataSize.z));
}

template<typename T>
const Range<typename Grid<T>::flat_iterator> Grid<T>::Items() {
	return MakeRange<std::vector<T>>(data);
}

template<typename T>
const Range<typename Grid<T>::const_flat_iterator> Grid<T>::Items() const {
	return MakeRange<const std::vector<T>>(data);
}

template<typename T>
typename Grid<T>::reference Grid<T>::At(vec3u pos) {
	return data[flatIndex(pos, dataSize.x, dataSize.y)];
}

template<typename T>
typename Grid<T>::reference Grid<T>::At(uint x, uint y, uint z) {
	return At({x,y,z});
}

template<typename T>
typename Grid<T>::const_reference Grid<T>::At(vec3u pos) const {
	return data[flatIndex(pos, dataSize.x, dataSize.y)];
}

template<typename T>
typename Grid<T>::const_reference Grid<T>::At(uint x, uint y, uint z) const {
	return At({x,y,z});
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
uint Grid<T>::flatIndex (const vec3u xyz, uint w, uint h) {
	return xyz.z * h * w + xyz.y * w + xyz.x;
}

} // namespace uf
