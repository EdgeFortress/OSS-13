#include "Button.hpp"

#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"

Button::Button(sf::Vector2f size) 
	: Widget(size)
{ 
	underCursor = false;
}

Button::Button(const sf::String &string,
			   const sf::Vector2f &size, 
	           std::function<void()> onPressFunc)
    : Widget(size),onPressFunc(onPressFunc) 
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

void Button::Update(sf::Time timeElapsed) {
	Style &tmpstyle = underCursor ? underCursor_style : this->style;
	if (tmpstyle.updated) {
		text.setFont(*tmpstyle.font);
		text.setCharacterSize(tmpstyle.fontSize);
		text.setFillColor(tmpstyle.textColor);
		text.setOutlineColor(tmpstyle.textColor);
		tmpstyle.updated = false;
	}
}

bool Button::HandleEvent(sf::Event event) {
    switch (event.type) {
    case sf::Event::MouseButtonPressed: {
        uf::vec2i mousePosition = uf::vec2i(event.mouseButton.x, event.mouseButton.y);
        if (mousePosition >= GetAbsPosition() && mousePosition < GetAbsPosition() + GetSize()) {
            onPressFunc();
            return true;
        }
        break;
    }
    case sf::Event::MouseMoved: {
        uf::vec2i mousePosition = uf::vec2i(event.mouseMove.x, event.mouseMove.y);
        if (mousePosition >= GetAbsPosition() && mousePosition < GetAbsPosition() + GetSize()) {
            if (!underCursor) {
                underCursor = true;
                underCursor_style.updated = true;
            }
            return true;
        }
        break;
    }
	case sf::Event::MouseLeft: {
		if (underCursor) {
			underCursor = false;
			style.updated = true;
		}
	}
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