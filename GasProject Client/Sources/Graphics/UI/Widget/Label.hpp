#pragma once

#include "Widget.hpp"

class Label : public Widget {
private:
	sf::Text text;

public:
	Label(const sf::String &, const sf::Font &);
	virtual void Draw(sf::RenderTarget &) const override;
	virtual void Update() override;
};