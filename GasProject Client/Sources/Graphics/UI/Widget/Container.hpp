#pragma once

#include <list>
#include "Widget.hpp"
#include "Common/Useful.hpp"

class Container : public Widget {
private:
	sf::Vector2f size;
	sf::RenderTexture buffer;
	//uptr<sf::Texture> buffer;
	std::list<std::pair<sf::Vector2f, uptr<Widget>>> items;

protected:
	virtual void draw() const override;

public:
	Container(const sf::Vector2f &size = sf::Vector2f());
	
	virtual void Update() final;

	void AddItem(Widget *, sf::Vector2f position);

	virtual void SetPosition(const sf::Vector2f) final;
	virtual void SetPosition(const float x, const float y) final;
};