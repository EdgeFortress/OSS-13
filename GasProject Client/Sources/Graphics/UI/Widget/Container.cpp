#include "Container.hpp"

Container::Container() {

}

void Container::Draw(sf::RenderTarget &target) const {
	for (auto &item : items) item.second->Draw(target);
};

void Container::AddItem(Widget *widget, sf::Vector2f position) {
	items.push_back(std::pair<sf::Vector2f, uptr<Widget>>(position, uptr<Widget>(widget)));
	widget->SetPosition(position + GetPosition());
}

void Container::SetPosition(const sf::Vector2f pos) {
	for (auto &item : items) item.second->SetPosition(pos + item.first);
	Widget::SetPosition(pos);
}
void Container::SetPosition(const float x, const float y) {
	for (auto &item : items) item.second->SetPosition(x + item.first.x, y + item.first.y);
	Widget::SetPosition(x, y);
}

void Container::Update() { }