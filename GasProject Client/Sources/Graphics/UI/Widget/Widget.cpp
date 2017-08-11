#include "Widget.hpp"

#include "Graphics/Window.hpp"
#include "Client.hpp"

Widget::Widget(sf::Vector2f size) : parent(nullptr) {
    bufferSprite.setTexture(buffer.getTexture());
	SetSize(size);
}

void Widget::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Draw(target);
}

void Widget::Draw(sf::RenderTarget &target) const {
	if (hiding) return;
	draw();
    bufferSprite.setTexture(buffer.getTexture(), true);
    target.draw(bufferSprite);
}

void Widget::Hide() { hiding = true; }
void Widget::Show() { hiding = false; }
bool Widget::IsVisible() { return !hiding; }

void Widget::SetPosition(const sf::Vector2f pos) {
	setPosition(pos);
    bufferSprite.setPosition(pos);
}
void Widget::SetPosition(const float x, const float y) {
	setPosition(x, y);
    bufferSprite.setPosition(x, y);
}
sf::Vector2f Widget::GetPosition() const {
	return getPosition();
}
sf::Vector2f Widget::GetAbsPosition() const {
	return parent ? getPosition() + parent->GetAbsPosition() : getPosition();
}

void Widget::SetSize(const sf::Vector2f &size) {
	this->size = size;
	if (size.x && size.y)
		if (!buffer.create(unsigned(size.x), unsigned(size.y)))
			CC::log << "Error: Cannot create Widget buffer" << std::endl;
}
sf::Vector2f Widget::GetSize() const {
	return size;
}

void Widget::setParent(Widget *widget) { parent = widget; }
