#include "SpriteFactory.h"

#include <Client.hpp>

#include <Shared/Global.hpp>

Sprite SpriteFactory::CreateSprite(uint32_t id) {
	if (!id--) // should be from 1 to Global::MAX_SPRITE_ID! Decrease it to use as index
		throw std::exception(); // "ID is null!"

	auto state = static_cast<Global::ItemSpriteState>(id & uint32_t(Global::ItemSpriteState::MASK));
	id -= uint32_t(state);
	
	if (id >= 0 && id < recipes.size()) {
		Sprite sprite;

		sprite.key = recipes[id].title;

		switch (state) {
			case Global::ItemSpriteState::DEFAULT: {
				sprite.texture = recipes[id].texture;
				sprite.firstFrame = recipes[id].firstFrame;
				sprite.frames = recipes[id].frames;
				sprite.frameTime = recipes[id].frameTime;
				sprite.directed = recipes[id].directed;
				break;
			};
			case Global::ItemSpriteState::ON_MOB: {
				sprite.texture = recipes[id].mobState_texture;
				sprite.firstFrame = recipes[id].mobState_firstFrame;
				sprite.directed = true;
				break;
			}
			case Global::ItemSpriteState::IN_HAND_LEFT:
			{
				sprite.texture = recipes[id].lhandState_texture;
				sprite.firstFrame = recipes[id].lhandState_firstFrame;
				sprite.directed = true;
				break;
			}
			case Global::ItemSpriteState::IN_HAND_RIGHT:
			{
				sprite.texture = recipes[id].rhandState_texture;
				sprite.firstFrame = recipes[id].rhandState_firstFrame;
				sprite.directed = true;
				break;
			}
			default:
				throw std::exception(); // "Sprite state is unknown!"
		}

		sprite.updateSpriteVariables();
		return sprite;
	}

	throw std::exception(); // "ID miss!"
}
