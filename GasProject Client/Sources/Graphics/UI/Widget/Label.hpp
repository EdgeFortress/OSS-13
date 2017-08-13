#pragma once

#include "Widget.hpp"

class Label : public Widget {
private:
	sf::Text text;

	sf::String string;
	sf::Color color;
	const sf::Font *font;
	unsigned size;
	bool needToUpdateText;

protected:
	virtual void draw() const override;

public:
	Label();
	Label(const sf::String &, const sf::Font &, const unsigned fontSize, const sf::Color & = sf::Color::White);
	
	virtual void Update(sf::Time timeElapsed) override;
	virtual bool HandleEvent(sf::Event event) override;

	void SetString(const sf::String &string);
    void SetFont(const sf::Font &font);
	void SetColor(const sf::Color &color);
	void SetFontSize(unsigned size);
};