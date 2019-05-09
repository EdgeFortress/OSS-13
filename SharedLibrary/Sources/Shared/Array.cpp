#include "Array.hpp"

namespace uf {
	uint flat_index (vec3u xyz, uint w, uint h) {
		return xyz.z * h * w + xyz.y * w + xyz.x;
	}
	uint flat_index (vec2u xy, uint w) {
		return xy.y * w + xy.x;
	}
}
