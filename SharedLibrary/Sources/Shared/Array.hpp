#pragma once

#include "Shared/Types.hpp"

namespace uf {
	uint flat_index (const vec3u xyz, uint w, uint h);
	uint flat_index (const vec2u xy, uint w);
}