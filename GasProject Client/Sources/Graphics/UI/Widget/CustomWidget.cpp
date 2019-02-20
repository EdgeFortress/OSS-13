#include "CustomWidget.h"

#include "Graphics/Window.hpp"
#include "Client.hpp"

CustomWidget::CustomWidget(uf::vec2i size) : parent(nullptr) {
	bufferSprite.setTexture(buffer.getTexture());
	CustomWidget::SetSize(size);
	canBeActive = false;
	active = false;
	hiding = false;
}

void CustomWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Draw(target);
}

void CustomWidget::Draw(sf::RenderTarget &target) const {
	if (hiding) return;
	draw();
	bufferSprite.setTexture(buffer.getTexture(), false);
	target.draw(bufferSprite);
}

void CustomWidget::Hide() { hiding = true; }
void CustomWidget::Show() { hiding = false; }

void CustomWidget::SetStyle(const Style &style) {
	this->style = style;
	this->style.updated = true;
}

void CustomWidget::SetStyle(Style &&) {
	this->style = style;
	this->style.updated = true;
}

void CustomWidget::SetPosition(const uf::vec2i &pos) {
	setPosition(pos);
	bufferSprite.setPosition(pos);
}
void CustomWidget::SetPosition(const float x, const float y) {
	setPosition(x, y);
	bufferSprite.setPosition(x, y);
}

void CustomWidget::SetSize(const uf::vec2i &size) {
	this->size = size;
	if (size.x && size.y) {
		if (!buffer.create(unsigned(size.x), unsigned(size.y)))
			LOGE << "Cannot create Widget buffer" << std::endl;
		bufferSprite.setTextureRect(sf::IntRect(0, 0, int(size.x), int(size.y)));
	}
}

bool CustomWidget::SetActive(bool active) {
	if (!canBeActive) return false;
	this->active = active;
	return true;
}

Style &CustomWidget::GetStyle() {
	style.updated = true;
	return style;
}

uf::vec2i CustomWidget::GetPosition() const {
	return uf::vec2i(getPosition());
}
uf::vec2i CustomWidget::GetAbsPosition() const {
	return parent ? uf::vec2i(getPosition()) + parent->GetAbsPosition() : uf::vec2i(getPosition());
}

uf::vec2i CustomWidget::GetSize() const { return size; }

bool CustomWidget::IsActive() const { return active; }

bool CustomWidget::IsVisible() const { return !hiding; }


void CustomWidget::setParent(Widget *widget) { parent = widget; }
