#include "Button.hpp"

#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"

Button::Button(sf::Vector2f size) 
	: Widget(size)
{
	needToUpdateText = false;
	underCursor = false;
	backgroundColor = sf::Color::Transparent;
}

Button::Button(const sf::String &string, 
				   sf::Vector2f &size, 
	            const sf::Color &textColor, 
	             const sf::Font &font, 
	                   unsigned  fontSize, 
	       std::function<void()> onPressFunc)
    : Widget(size), string(string), textColor(textColor), font(&font), onPressFunc(onPressFunc) 
{
	needToUpdateText = true;
	underCursor = false;
	backgroundColor = sf::Color::Transparent;
}

void Button::draw() const {
    sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
    buffer.clear(!underCursor ? backgroundColor : underCursor_backgroundColor);
    buffer.draw(text);
    buffer.display();
}

void Button::Update(sf::Time timeElapsed) {
	if (underCursor) {
		underCursor_time += timeElapsed;
		if (underCursor_time >= underCursor_newEventWaitingTime) {
			underCursor = false;
			needToUpdateText = true;
		}
	}

	if (needToUpdateText && font) {
		text = sf::Text(string, *font, fontSize);
		if (!underCursor) {
			text.setFillColor(textColor);
			text.setOutlineColor(textColor);
		} else {
			text.setFillColor(underCursor_textColor);
			text.setOutlineColor(underCursor_textColor);
		}
		needToUpdateText = false;
	}
}

bool Button::HandleEvent(sf::Event event) {
	switch (event.type) {
	    case sf::Event::MouseButtonPressed: {
		    uf::vec2i mousePosition = uf::vec2i(event.mouseButton.x, event.mouseButton.y);
            if (mousePosition >= GetAbsPosition() && mousePosition <= GetAbsPosition() + GetSize()) {
                onPressFunc();
                return true;
            }
		    break;
	    }
	    case sf::Event::MouseMoved: {
		    uf::vec2i mousePosition = uf::vec2i(event.mouseMove.x, event.mouseMove.y);
		    if (mousePosition < GetAbsPosition() && mousePosition >= GetAbsPosition() + GetSize()) {
			    underCursor = true;
			    underCursor_time = sf::Time::Zero;
			    needToUpdateText = true;
                return true;
		    }
		    break;
	    }
	}
    return false;
}

void Button::SetString(const sf::String &string) {
	this->string = string;
	needToUpdateText = true;
}
void Button::SetFont(const sf::Font &font) {
	this->font = &font;
	needToUpdateText = true;
}
void Button::SetTextColor(const sf::Color &color) {
	this->textColor = color;
	needToUpdateText = true;
}
void Button::SetFontSize(unsigned size) {
	this->fontSize = size;
	needToUpdateText = true;
}
