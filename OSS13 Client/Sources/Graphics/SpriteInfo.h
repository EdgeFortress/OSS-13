#pragma once

#include <string>

#include <SFML/System/Time.hpp>

class Texture;

struct SpriteInfo
{
	std::string title;

	const Texture *texture;
	uint32_t firstFrame{};
	uint32_t frames{};
	sf::Time frameTime;
	bool directed{};
	bool pureDirections{};

	const Texture *mobState_texture{};
	uint32_t mobState_firstFrame{};

	const Texture *lhandState_texture{};
	uint32_t lhandState_firstFrame{};

	const Texture *rhandState_texture{};
	uint32_t rhandState_firstFrame{};
};
