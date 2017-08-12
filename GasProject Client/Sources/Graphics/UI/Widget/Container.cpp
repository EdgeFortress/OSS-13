#include "Container.hpp"

#include <typeinfo>
#include <algorithm>

#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"
#include "Entry.hpp"

Container::Container(sf::Vector2f size) :
	Widget(size), backgroundColor(sf::Color::Transparent), curInputWidgetIterator(items.end())
{ }

void Container::Update(sf::Time timeElapsed) {
	for (auto &item : items)
		item->Update(timeElapsed);
}

bool Container::HandleEvent(sf::Event event) {
	if (!IsVisible()) return false;
	switch (event.type) {
	case sf::Event::MouseButtonPressed: {
		uf::vec2i mousePosition = uf::vec2i(event.mouseButton.x, event.mouseButton.y);
		if (!(mousePosition >= GetAbsPosition() && mousePosition < GetAbsPosition() + GetSize()))
			return false;

		for (auto iter = items.begin(); iter != items.end(); iter++) {
			Widget *widget = iter->get();
			if (widget->HandleEvent(event)) {
				if (widget->SetActive(active)) {
					if (curInputWidgetIterator->get() == widget) return true;
					curInputWidgetIterator->get()->SetActive(false);
					curInputWidgetIterator = iter;
					return true;
				}
			}
		}
		return true;
	}
	case sf::Event::MouseMoved: {
		uf::vec2i mousePosition = uf::vec2i(event.mouseMove.x, event.mouseMove.y);
		if (mousePosition >= GetAbsPosition() && mousePosition < GetAbsPosition() + GetSize())
			return false;
		for (auto &widget : items)
			if (widget->HandleEvent(event)) return true;
		return true;
	}
	case sf::Event::KeyPressed: {
		if (event.key.code == sf::Keyboard::Tab) {
			if (curInputWidgetIterator == items.end()) return true;
			curInputWidgetIterator->get()->SetActive(false);

			// find next entry
			auto iter = curInputWidgetIterator;
			iter++;
			while (!iter->get()->SetActive(true) && iter != curInputWidgetIterator) {
				iter++;
				if (iter == items.end())
					iter = items.begin();
			}
			curInputWidgetIterator = iter;

			return true;
		}
		if (curInputWidgetIterator != items.end())
			curInputWidgetIterator->get()->HandleEvent(event);
		return true;
	}
	default:
		if (curInputWidgetIterator != items.end())
			curInputWidgetIterator->get()->HandleEvent(event);
	}
	return true;
}

void Container::draw() const {
	buffer.clear(backgroundColor);
    for (auto &item : items)
        buffer.draw(*(item.get()));
	buffer.display();
}

void Container::AddItem(Widget *widget, sf::Vector2f position) {
	items.push_back(uptr<Widget>(widget));
    if (curInputWidgetIterator == items.end())
		if (widget->SetActive(false)) { // Check if widget can be active
			canBeActive = true;
			curInputWidgetIterator = items.end();
			curInputWidgetIterator--;
		}
	widget->SetPosition(position);
    widget->setParent(this);
}

void Container::Clear() {
    items.erase(items.begin(), items.end());
	canBeActive = false;
	curInputWidgetIterator = items.end();
}

void Container::SetBackground(sf::Color color) {
    backgroundColor = color;
}

bool Container::SetActive(bool active) {
	if (curInputWidgetIterator != items.end())
		curInputWidgetIterator->get()->SetActive(active);
	return Widget::SetActive(active);
}
