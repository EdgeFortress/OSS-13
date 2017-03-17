#pragma once

#include "Widget.hpp"

class Label : public Widget {
private:
	sf::Text text;

protected:
	virtual void draw() const override;

public:
	Label(const sf::String &, const sf::Font &, const unsigned &size, const sf::Color & = sf::Color::White);
	
	virtual void Update() override;

	virtual void SetPosition(const sf::Vector2f) override;
	virtual void SetPosition(const float x, const float y) override;
};