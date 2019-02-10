#pragma once

#include <Graphics/UI/Widget/Widget.hpp>

class CustomWidget : public Widget {
public:
	virtual void Update(sf::Time timeElapsed) = 0;
	virtual bool HandleEvent(sf::Event event) = 0;

protected:
	virtual void draw() const = 0;
};
