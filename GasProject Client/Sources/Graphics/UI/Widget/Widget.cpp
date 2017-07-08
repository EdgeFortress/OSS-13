#include "Widget.hpp"

#include "Graphics/Window.hpp"

Widget::Widget(const sf::Vector2f &size) : size(size) {
	if (size.x || size.y)
		buffer.create(unsigned(size.x), unsigned(size.y));
    bufferSprite.setTexture(buffer.getTexture());
}

void Widget::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Draw(target);
}

void Widget::Draw(sf::RenderTarget &target) const {
	draw();
    auto &bufferSprite = const_cast<sf::Sprite &>(this->bufferSprite);
    bufferSprite.setTexture(buffer.getTexture(), true);
    target.draw(bufferSprite);
}

void Widget::SetPosition(const sf::Vector2f pos) {
	setPosition(pos);
    bufferSprite.setPosition(pos);
}
void Widget::SetPosition(const float x, const float y) {
	setPosition(x, y);
    bufferSprite.setPosition(x, y);
}
const sf::Vector2f Widget::GetPosition() const {
	return getPosition();
}

void Widget::SetSize(const sf::Vector2f &size) {
	this->size = size;
	if (!buffer.create(unsigned(size.x), unsigned(size.y)))
		CC::log << "error" << endl;
}
const sf::Vector2f Widget::GetSize() const {
	return size;
}
