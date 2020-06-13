#pragma once

#include <vector>

#include <Graphics/Texture.hpp>

class TextureHolder {
public:
	Texture *LoadTexture(const std::string &path, uint32_t tileSize);

protected:
	std::vector<uptr<Texture>> textures;
};
