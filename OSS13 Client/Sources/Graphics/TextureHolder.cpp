#include "TextureHolder.h"

Texture *TextureHolder::LoadTexture(std::wstring path, uint32_t tileSize) {
	textures.push_back(std::make_unique<Texture>());

	Texture *texture = textures.back().get();
	if (!texture->load(path, tileSize)) {
		textures.pop_back();
		throw std::exception(); // "TextureHolder::CreateTexture error! Texture loading failed."
	}

	return texture;
}
