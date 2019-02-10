#pragma once

#include <Graphics/UI/Widget/Widget.hpp>

class ImGuiWidget : public Widget {
public:
	virtual void Update(sf::Time timeElapsed) = 0;
	virtual bool HandleEvent(sf::Event event) final;

protected:
	virtual void draw() const final;
};
