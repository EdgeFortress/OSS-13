#pragma once

#include "Widget.hpp"

class Label : public Widget {
private:
	sf::Text text;
    sf::Font font;

    sf::String name;

protected:
	virtual void draw() const override;

public:
	Label(const sf::String &, const sf::Font &, const unsigned &size, const sf::Color & = sf::Color::White);
    Label(const sf::String &, const unsigned &size, const sf::Color & = sf::Color::White);
	
	virtual void Update(sf::Time timeElapsed) override;

	virtual void SetPosition(const sf::Vector2f) override;
	virtual void SetPosition(const float x, const float y) override;

    virtual void SetFont(const sf::Font &font);
};