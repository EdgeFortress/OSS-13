#include "Widget.hpp"

void Widget::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Draw(target);
}

void Widget::SetPosition(const sf::Vector2f pos) {
	setPosition(pos);
}
void Widget::SetPosition(const float x, const float y) {
	setPosition(x, y);
}
const sf::Vector2f Widget::GetPosition() const {
	return getPosition();
}


