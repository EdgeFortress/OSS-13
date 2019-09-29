#include "SpriteFactory.h"

#include <Client.hpp>

#include <Shared/Global.hpp>
#include <Shared/ErrorHandling.h>

Sprite SpriteFactory::CreateSprite(uint32_t id) {
	EXPECT_WITH_MSG(id--, "ID is null!");

	auto state = static_cast<Global::ItemSpriteState>(id & uint32_t(Global::ItemSpriteState::MASK));
	id -= uint32_t(state);
	
	EXPECT_WITH_MSG(id < recipes.size(), "ID miss!");

	return Sprite(recipes[id], state);
}
