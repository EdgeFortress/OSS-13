#include "Container.hpp"

#include <typeinfo>
#include <algorithm>

#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"
#include "Entry.hpp"

Container::Container(const uf::vec2i &size) :
	CustomWidget(size),
    curInputWidgetIterator(items.end()), underCursorWidget(nullptr)
{ }

void Container::Update(sf::Time timeElapsed) {
	for (auto &item : items)
		item->Update(timeElapsed);
}

bool Container::HandleEvent(sf::Event event) {
	if (!IsVisible()) return false;
	return CustomWidget::HandleEvent(event);
}

bool Container::OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) {
	if (!(position >= GetAbsPosition() && position < GetAbsPosition() + GetSize()))
		return false;

	for (auto iter = items.begin(); iter != items.end(); iter++) {
		CustomWidget *widget = iter->get();
		if (widget->OnMouseButtonPressed(button, position)) {
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

bool Container::OnMouseMoved(uf::vec2i position) {
	if (!(position >= GetAbsPosition() && position < GetAbsPosition() + GetSize()))
		return false;

	for (auto &widget : items) {
		if (widget->OnMouseMoved(position)) {
			if (underCursorWidget && underCursorWidget != widget.get()) {
				sf::Event event;
				event.type = sf::Event::MouseLeft;
				underCursorWidget->HandleEvent(event);
			}
			underCursorWidget = widget.get();
			return true;
		}
	}

	if (underCursorWidget) {
		sf::Event event;
		event.type = sf::Event::MouseLeft;
		underCursorWidget->HandleEvent(event);
		underCursorWidget = nullptr;
	}

	return true;
}

bool Container::OnMouseLeft() {
	if (underCursorWidget)
		underCursorWidget->OnMouseLeft();

	return true;
}

bool Container::OnKeyPressed(sf::Event::KeyEvent keyEvent) {
	if (keyEvent.code == sf::Keyboard::Tab) {
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
		curInputWidgetIterator->get()->OnKeyPressed(keyEvent);
	return true;
}

bool Container::OnTextEntered(uint32_t unicodeChar) {
	if (curInputWidgetIterator != items.end())
		curInputWidgetIterator->get()->OnTextEntered(unicodeChar);
	return true;
}

void Container::drawContainer() const {
	return;
}

void Container::draw() const {
	buffer.clear(style.backgroundColor);
	drawContainer();
	for (auto &item : items)
		buffer.draw(*(item.get()));
	buffer.display();
}

void Container::AddItem(CustomWidget *widget, const uf::vec2i &position) {
	items.push_back(uptr<CustomWidget>(widget));
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

bool Container::SetActive(bool active) {
	if (curInputWidgetIterator != items.end())
		curInputWidgetIterator->get()->SetActive(active);
	return CustomWidget::SetActive(active);
}
