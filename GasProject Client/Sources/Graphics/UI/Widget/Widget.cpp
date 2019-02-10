#include "Widget.hpp"

#include "Graphics/Window.hpp"
#include "Client.hpp"

Widget::Widget(uf::vec2i size) : parent(nullptr) {
    bufferSprite.setTexture(buffer.getTexture());
	Widget::SetSize(size);
	canBeActive = false;
	active = false;
	hiding = false;
}

void Widget::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Draw(target);
}

void Widget::Draw(sf::RenderTarget &target) const {
	if (hiding) return;
	draw();
    bufferSprite.setTexture(buffer.getTexture(), false);
    target.draw(bufferSprite);
}

void Widget::Hide() { hiding = true; }
void Widget::Show() { hiding = false; }

void Widget::SetStyle(const Style &style) {
    this->style = style;
    this->style.updated = true;
}

void Widget::SetStyle(Style &&) {
    this->style = style;
    this->style.updated = true;
}

void Widget::SetPosition(const uf::vec2i &pos) {
	setPosition(pos);
    bufferSprite.setPosition(pos);
}
void Widget::SetPosition(const float x, const float y) {
	setPosition(x, y);
    bufferSprite.setPosition(x, y);
}

void Widget::SetSize(const uf::vec2i &size) {
	this->size = size;
	if (size.x && size.y) {
		if (!buffer.create(unsigned(size.x), unsigned(size.y)))
			LOGE << "Cannot create Widget buffer" << std::endl;
		bufferSprite.setTextureRect(sf::IntRect(0, 0, int(size.x), int(size.y)));
	}
}

bool Widget::SetActive(bool active) {
	if (!canBeActive) return false;
	this->active = active;
	return true;
}

Style &Widget::GetStyle() { 
    style.updated = true;
    return style; 
}

uf::vec2i Widget::GetPosition() const {
    return uf::vec2i(getPosition());
}
uf::vec2i Widget::GetAbsPosition() const {
    return parent ? uf::vec2i(getPosition()) + parent->GetAbsPosition() : uf::vec2i(getPosition());
}

uf::vec2i Widget::GetSize() const { return size; }

bool Widget::IsActive() const { return active; }

bool Widget::IsVisible() const { return !hiding; }

void Widget::setParent(Widget *widget) { parent = widget; }
