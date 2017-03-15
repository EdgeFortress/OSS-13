#pragma once

#include <SFML/Graphics.hpp>

class Widget : private sf::Transformable, private sf::Drawable {
private:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const final;

public:
	virtual void Draw(sf::RenderTarget &target) const = 0;
	virtual void Update() = 0;

	virtual void SetPosition(const sf::Vector2f);
	virtual void SetPosition(const float x, const float y);
	virtual const sf::Vector2f GetPosition() const;
};