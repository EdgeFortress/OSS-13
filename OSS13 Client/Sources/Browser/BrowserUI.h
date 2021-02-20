#pragma once

#include <SFML/Window/WindowHandle.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

class BrowserUI
{
public:
	virtual uf::vec2i CastViewPointToScreenPoint(uf::vec2i viewPoint) = 0;

	virtual sf::WindowHandle GetWindowHandle() = 0;
	virtual sf::RenderTexture &GetRenderTexture() = 0;
};
