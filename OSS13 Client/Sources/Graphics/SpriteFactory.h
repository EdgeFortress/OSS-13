#pragma once

#include <vector>

#include <Graphics/Sprite.hpp>
#include <Graphics/SpriteInfo.h>

class SpriteFactory {
public:
	// id from 1 to Global::MAX_SPRITE_ID
	Sprite CreateSprite(uint32_t id);

protected:
	std::vector<uptr<Sprite>> sprites;
	std::vector<SpriteInfo> recipes;
};
