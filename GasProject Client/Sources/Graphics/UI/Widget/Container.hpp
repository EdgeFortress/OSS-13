#pragma once

#include <list>
#include "Widget.hpp"
#include "Common/Useful.hpp"

class Container : public Widget {
private:
	std::list<std::pair<sf::Vector2f, uptr<Widget>>> items;

public:
	Container() {

	}

	virtual void Draw(sf::RenderTarget &target) const override {
		for (auto &item : items) item.second->Draw(target);
	};

	void AddItem(Widget *widget, sf::Vector2f position) {
		items.push_back(std::pair<sf::Vector2f, uptr<Widget>>(position, uptr<Widget>(widget)));
		widget->SetPosition(position + GetPosition());
	}

	virtual void SetPosition(const sf::Vector2f pos) final {
		for (auto &item : items) item.second->SetPosition(pos + item.first);
		Widget::SetPosition(pos);
	}
	virtual void SetPosition(const float x, const float y) final {
		for (auto &item : items) item.second->SetPosition(x + item.first.x, y + item.first.y);
		Widget::SetPosition(x, y);
	}
	virtual void Update() final { }
};