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
    text.setString(string);
	underCursor = false;
}

void Button::draw() const {
    buffer.clear(style.backgroundColor);
    buffer.draw(text);
    buffer.display();
}

void Button::Update(sf::Time timeElapsed) {
	if (underCursor) {
		underCursor_time += timeElapsed;
		if (underCursor_time >= underCursor_newEventWaitingTime) {
			underCursor = false;
            style = std::move(bufferStyle);
            style.updated = true;
		}
	}

    if (GetStyle().updated) {
        text.setFont(*style.font);
        text.setCharacterSize(style.fontSize);
        text.setFillColor(GetStyle().textColor);
        text.setOutlineColor(GetStyle().textColor);
        GetStyle().updated = false;
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
            underCursor = true;
            underCursor_time = sf::Time::Zero;
            bufferStyle = std::move(style);
            style = underCursor_Style;
            style.updated = true;
            return true;
        }
        break;
    }
    }
    return false;
}

void Button::SetString(const sf::String &string) {
    text.setString(string);
}

void Button::SetUnderCursorStyle(const Style &style) {
    underCursor_Style = style;
    underCursor_Style.updated = true;
}

void Button::SetUnderCursorStyle(Style &&style) {
    underCursor_Style = style;
    underCursor_Style.updated = true;
}