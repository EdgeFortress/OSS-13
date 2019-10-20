#include "Sprite.hpp"

#include <SFML/Graphics.hpp>

#include <Shared/Geometry/Angle.h>
#include <Shared/ErrorHandling.h>

#include <Client.hpp>

#include "Window.hpp"

namespace {

uf::Angle getSpriteAngle(uf::Direction direction) {
	if (direction == uf::Direction::NONE)
		direction = uf::Direction::SOUTH;
	// for uf::Direction::SOUTH should return zero angle
	return uf::DirectionToAngle(direction) + uf::Angle::Degrees(90);
}

} // namespace

Sprite::Sprite(const SpriteInfo &recipe, Global::ItemSpriteState state) {
	initializeFromRecipe(recipe, state);
	updateSpriteVariables();
}

void Sprite::Draw(sf::RenderTarget *target, uf::vec2f pos, sf::RenderStates rs) const {
	sfSprite.setPosition(pos + origin);
	target->draw(sfSprite, rs);
}

void Sprite::Resize(int tileSize) {
	scale = float(tileSize) / texture->GetSizeOfTile();
	sfSprite.setScale(scale, scale);
}

bool Sprite::Update(sf::Time timeElapsed) {
	if (frames > 1) {
		curFrameTime += timeElapsed;
		if (curFrameTime >= frameTime) {
			curFrameTime = sf::Time();
			curFrame++;
			if (curFrame >= frames) {
				curFrame = 0;
				updateSpriteVariables();
				return true;
			}
			updateSpriteVariables();
		}
		return false;
	}
	return true;
}

void Sprite::SetDirection(uf::Direction direction) {
	this->direction = direction;
	updateSpriteVariables();
}

const std::string &Sprite::GetKey() const { return key; }
bool Sprite::IsValid() const { return texture; }
bool Sprite::IsAnimated() const { return frames > 1; }

bool Sprite::PixelTransparent(uf::vec2u pixel) const {
	pixel = pixel / scale;
	if (directed && pureDirections)
		pixel = (pixel - origin).rotate(-getSpriteAngle(direction)) + origin;
	if (texture->IsFramePixelTransparent(pixel, curFrame + firstFrame)) return true;
	return false;
}

const sf::Sprite &Sprite::GetSfmlSprite() const {
	return sfSprite;
}

void Sprite::initializeFromRecipe(const SpriteInfo &recipe, Global::ItemSpriteState state) {
	key = recipe.title;

	switch (state) {
		case Global::ItemSpriteState::DEFAULT:
		{
			texture = recipe.texture;
			firstFrame = recipe.firstFrame;
			frames = recipe.frames;
			frameTime = recipe.frameTime;
			directed = recipe.directed;
			pureDirections = recipe.pureDirections;
			break;
		};
		case Global::ItemSpriteState::ON_MOB:
		{
			texture = recipe.mobState_texture;
			firstFrame = recipe.mobState_firstFrame;
			directed = true;
			break;
		}
		case Global::ItemSpriteState::IN_HAND_LEFT:
		{
			texture = recipe.lhandState_texture;
			firstFrame = recipe.lhandState_firstFrame;
			directed = true;
			break;
		}
		case Global::ItemSpriteState::IN_HAND_RIGHT:
		{
			texture = recipe.rhandState_texture;
			firstFrame = recipe.rhandState_firstFrame;
			directed = true;
			break;
		}
		default:
			EXPECT_WITH_MSG(false, "Unknown state: " + std::to_string(static_cast<uint32_t>(state)));
	}
}

void Sprite::updateSpriteVariables() {
	if (!texture)
		return;

	int realState = firstFrame;
	if (directed && direction != uf::Direction::NONE) {
		if (!pureDirections)
			realState += int(direction) * frames;
		else {
			sfSprite.setRotation(static_cast<float>(getSpriteAngle(direction).GetDegrees()));
		}
	}
	if (frames > 1) realState += curFrame;

	rect.left = realState % texture->GetNumOfTiles().x * texture->GetSizeOfTile();
	rect.top = realState / texture->GetNumOfTiles().x * texture->GetSizeOfTile();
	rect.width = rect.height = texture->GetSizeOfTile();

	origin = uf::vec2f(static_cast<float>(texture->GetSizeOfTile())) / 2;

	sfSprite.setTexture(*texture->GetSFMLTexture());
	sfSprite.setTextureRect(rect);
	sfSprite.setOrigin(origin);
}
