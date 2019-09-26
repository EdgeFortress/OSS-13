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

void CustomWidget::Draw(sf::RenderTarget &target) const {
	if (hiding) return;
	draw();
	bufferSprite.setTexture(buffer.getTexture(), false);
	target.draw(bufferSprite);
}

bool CustomWidget::HandleEvent(sf::Event event) {
	switch (event.type) {
		case sf::Event::MouseButtonPressed:
			return OnMouseButtonPressed(event.mouseButton.button, { event.mouseButton.x, event.mouseButton.y });
		case sf::Event::MouseMoved:
			return OnMouseMoved({ event.mouseMove.x, event.mouseMove.y });
		case sf::Event::MouseLeft:
			return OnMouseLeft();
		case sf::Event::MouseWheelScrolled:
			return OnMouseWheelScrolled(event.mouseWheelScroll.delta, { event.mouseWheelScroll.x,  event.mouseWheelScroll.y });
		case sf::Event::KeyPressed:
			return OnKeyPressed(event.key);
		case sf::Event::KeyReleased:
			return OnKeyReleased(event.key.code);
		case sf::Event::TextEntered:
			return OnTextEntered(event.text.unicode);
		default:
			break;
	};
	return false;
}

bool CustomWidget::OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) { return false; }
bool CustomWidget::OnMouseMoved(uf::vec2i position) { return false; }
bool CustomWidget::OnMouseLeft() { return false; }
bool CustomWidget::OnMouseWheelScrolled(float delta, uf::vec2i position) { return false; }
bool CustomWidget::OnKeyPressed(sf::Event::KeyEvent keyEvent) { return false; }
bool CustomWidget::OnKeyReleased(sf::Keyboard::Key button) { return false; }
bool CustomWidget::OnTextEntered(uint32_t unicodeChar) { return false; }

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

void CustomWidget::SetScale(const uf::vec2f &scale) {
	setScale(scale);
	bufferSprite.setScale(scale);
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

Widget *CustomWidget::GetParent() const {
	return parent;
}

uf::vec2i CustomWidget::GetPosition() const {
	return uf::vec2i(getPosition());
}

uf::vec2i CustomWidget::GetAbsolutePosition() const {
	return parent ? uf::vec2i(getPosition()) + parent->GetAbsolutePosition() : uf::vec2i(getPosition());
}

uf::vec2i CustomWidget::GetSize() const { return size; }

uf::vec2i CustomWidget::GetAbsoluteSize() const { 
	const Widget *widget = this;
	uf::vec2i size = GetSize();
	while (widget) {
		size = uf::vec2f(size.x * parent->GetScale().x, size.y * parent->GetScale().y);
		widget = widget->GetParent();
	}
	return size;
}

uf::vec2f CustomWidget::GetScale() const { return getScale(); }

bool CustomWidget::IsActive() const { return active; }

bool CustomWidget::IsVisible() const { return !hiding; }

void CustomWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Draw(target);
}

void CustomWidget::setParent(Widget *widget) { parent = widget; }
