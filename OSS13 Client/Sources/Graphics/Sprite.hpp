#pragma once

#include <Shared/Global.hpp>
#include <Shared/Types.hpp>
#include <Shared/IFaces/ICopyable.h>

#include "Texture.hpp"
#include "SpriteInfo.h"

class Sprite : public ICopyable {
public:
	Sprite() = default;
	Sprite(const SpriteInfo &recipe, Global::ItemSpriteState state);

	void Draw(sf::RenderTarget *, uf::vec2i pos, sf::RenderStates rs = sf::RenderStates::Default) const;
	// true if frame changed to the first
	bool Update(sf::Time timeElapsed);
	void Resize(int tileSize);

	void SetDirection(uf::Direction direction);

	const std::string &GetKey() const;
	// true if sprite has few frames
	bool IsValid() const;
	bool IsAnimated() const;
	bool PixelTransparent(uf::vec2u pixel) const;
	const sf::Sprite &GetSfmlSprite() const;

private:
	void initializeFromRecipe(const SpriteInfo &recipe, Global::ItemSpriteState state);
	void updateSpriteVariables();

private:
	const Texture *texture{};
	std::string key;
	uint firstFrame{};
	uint frames{1};
	sf::Time frameTime;
	bool directed{};
	bool pureDirections{};

	uint curFrame{};
	uf::Direction direction{uf::Direction::NONE};
	float scale{1};
	sf::Rect<int> rect;
	uf::vec2f origin;
	sf::Time curFrameTime;

	mutable sf::Sprite sfSprite;
};
