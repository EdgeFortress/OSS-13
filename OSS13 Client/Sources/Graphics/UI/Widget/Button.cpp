#include "Button.hpp"

#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"

Button::Button(const uf::vec2i &size) 
	: CustomWidget(size)
{ 
	underCursor = false;
}

Button::Button(const sf::String &string,
               const uf::vec2i &size,
	           std::function<void()> onPressFunc)
    : CustomWidget(size),onPressFunc(onPressFunc)
{
	underCursor = false;
    text.setString(string);
}

void Button::draw() const {
	const Style &tmpstyle = underCursor ? underCursor_style : this->style;
    buffer.clear(tmpstyle.backgroundColor);
    buffer.draw(text);
    buffer.display();
}

void Button::Update(sf::Time /*timeElapsed*/) {
	Style &tmpstyle = underCursor ? underCursor_style : this->style;
	if (tmpstyle.updated) {
		text.setFont(*tmpstyle.font);
		text.setCharacterSize(tmpstyle.fontSize);
		text.setFillColor(tmpstyle.textColor);
		text.setOutlineColor(tmpstyle.textColor);
		tmpstyle.updated = false;
	}
}

bool Button::OnMouseButtonPressed(sf::Mouse::Button, uf::vec2i position) {
	if (position >= GetAbsolutePosition() && position < GetAbsolutePosition() + GetSize()) {
		onPressFunc();
		return true;
	}
	return false;
}

bool Button::OnMouseMoved(uf::vec2i position) {
	if (position >= GetAbsolutePosition() && position < GetAbsolutePosition() + GetSize()) {
		if (!underCursor) {
			underCursor = true;
			underCursor_style.updated = true;
		}
		return true;
	}
	return false;
}

bool Button::OnMouseLeft() {
	if (underCursor) {
		underCursor = false;
		style.updated = true;
	}
	return false;
}

void Button::SetString(const sf::String &string) {
    text.setString(string);
}

void Button::SetUnderCursorStyle(const Style &style) {
    underCursor_style = style;
    underCursor_style.updated = true;
}

void Button::SetUnderCursorStyle(Style &&style) {
    underCursor_style = style;
    underCursor_style.updated = true;
}