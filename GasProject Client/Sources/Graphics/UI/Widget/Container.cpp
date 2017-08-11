#include "Container.hpp"

#include <typeinfo>
#include <algorithm>

#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"
#include "Entry.hpp"

Container::Container(sf::Vector2f size) :
	Widget(size), backgroundColor(sf::Color::Transparent), curInputEntryIterator(items.end())
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
		if (mousePosition < GetAbsPosition() && mousePosition >= GetAbsPosition() + GetSize())
			return false;

		for (auto iter = items.begin(); iter != items.end(); iter++) {
			Widget *widget = iter->get();
			if (widget->HandleEvent(event)) {
				if (typeid(*widget).hash_code() == typeid(Entry).hash_code()) {
					if (curInputEntryIterator->get() == widget) return true;
					Entry *lastEntry = reinterpret_cast<Entry *>(curInputEntryIterator->get());
					lastEntry->SetActive(false);
					curInputEntryIterator = iter;
					Entry *newEntry = reinterpret_cast<Entry *>(curInputEntryIterator->get());
					newEntry->SetActive(true);
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
			if (curInputEntryIterator == items.end()) return true;
			Entry *lastEntry = reinterpret_cast<Entry *>(curInputEntryIterator->get());
			lastEntry->SetActive(false);

			// find next entry
			auto iter = curInputEntryIterator;
			iter++;
			while (iter != curInputEntryIterator && typeid(*iter->get()) != typeid(Entry)) {
				iter++;
				if (iter == items.end())
					iter = items.begin();
			}
			curInputEntryIterator = iter;

			Entry *newEntry = reinterpret_cast<Entry *>(curInputEntryIterator->get());
			newEntry->SetActive(true);
			return true;
		}
		if (curInputEntryIterator != items.end())
			curInputEntryIterator->get()->HandleEvent(event);
		return true;
	}
	default:
		if (curInputEntryIterator != items.end())
			curInputEntryIterator->get()->HandleEvent(event);
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
    if (curInputEntryIterator == items.end())
		if (typeid(*widget).hash_code() == typeid(Entry).hash_code()) {
			curInputEntryIterator = items.end();
			curInputEntryIterator--;
		}
	widget->SetPosition(position);
    widget->setParent(this);
}

void Container::Clear() {
    items.erase(items.begin(), items.end());
	curInputEntryIterator = items.end();
}

void Container::SetBackground(sf::Color color) {
    backgroundColor = color;
}
