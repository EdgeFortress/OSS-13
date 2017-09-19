#include "Label.hpp"

#include "Client.hpp"

Label::Label() {
	Hide();
}

Label::Label(const sf::String &string) {
    text.setString(string);
}

void Label::Update(sf::Time timeElapsed) {
	if (style.updated) {
        text.setFont(*style.font);
        text.setCharacterSize(style.fontSize);
		text.setFillColor(GetStyle().textColor);
		text.setOutlineColor(GetStyle().textColor);
		SetSize(sf::Vector2f(text.getGlobalBounds().width + text.getGlobalBounds().left, text.getGlobalBounds().height + text.getGlobalBounds().top));
        GetStyle().updated = false;
		Show();
    }
}

bool Label::HandleEvent(sf::Event event) { return false; }

void Label::SetString(const sf::String &string) {
    text.setString(string);
}

void Label::draw() const {
    buffer.clear(sf::Color::Transparent);
    buffer.draw(text);
    buffer.display();
}
