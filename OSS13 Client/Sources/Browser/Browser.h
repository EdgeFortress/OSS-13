#pragma once

#include <memory>

#include <SFML/Window/Event.hpp>

#include <Shared/Geometry/Vec2.hpp>
#include <Shared/IFaces/IFace.h>

struct Browser : public IFace
{
	// set content in HTML format
	virtual void SetContent(const std::string& content) = 0;

	virtual bool OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) = 0;
	virtual bool OnMouseButtonReleased(sf::Mouse::Button button, uf::vec2i position) = 0;
	virtual bool OnMouseMoved(uf::vec2i position) = 0;
	virtual bool OnMouseLeft() = 0;
	virtual bool OnMouseWheelScrolled(float delta, uf::vec2i position) = 0;
	virtual bool OnKeyPressed(sf::Event::KeyEvent keyEvent) = 0;
	virtual bool OnKeyReleased(sf::Event::KeyEvent keyEvent) = 0;
	virtual bool OnTextEntered(uint32_t unicodeChar) = 0;
};
