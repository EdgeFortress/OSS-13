#include "Container.hpp"
#include "Client.hpp"

Container::Container(const sf::Vector2f &size) : Widget(size), backgroundColor(sf::Color::Transparent) { }

void Container::Update(sf::Time timeElapsed) { for (auto &item : items) item.second->Update(timeElapsed); }

void Container::HandleEvent(sf::Event event) { for (auto &item : items) item.second->HandleEvent(event); }

void Container::draw() const {
	sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
	buffer.clear(backgroundColor);
    for (auto &item : items) //item.second->Draw(buffer);
        buffer.draw(*(item.second.get()));
	buffer.display();
}

void Container::AddItem(Widget *widget, sf::Vector2f position) {
	items.push_back(std::pair<sf::Vector2f, uptr<Widget>>(position, uptr<Widget>(widget)));
	widget->SetPosition(position/* + GetPosition()*/);
    widget->SetParent(this);
}

void Container::SetPosition(const sf::Vector2f pos) {
	Widget::SetPosition(pos);
}
void Container::SetPosition(const float x, const float y) {
	Widget::SetPosition(x, y);
}

void Container::SetBackground(sf::Color color) {
    backgroundColor = color;
}