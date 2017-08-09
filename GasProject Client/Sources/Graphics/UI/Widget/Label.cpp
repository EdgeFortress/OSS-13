#include "Label.hpp"

#include "Client.hpp"

Label::Label() {
	Hide();
	font = nullptr;
	needToUpdateText = false;
}

Label::Label(const sf::String &string, const sf::Font &font, const unsigned size, const sf::Color &color) :
	string(string), font(&font), size(size), color(color)
{
	needToUpdateText = true;
}

void Label::draw() const {
	sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
	buffer.clear(sf::Color::Transparent);
	buffer.draw(text);
	buffer.display();
}

void Label::Update(sf::Time timeElapsed) {
	if (needToUpdateText && font) {
		text = sf::Text(string, *font, size);
		text.setFillColor(color);
		text.setOutlineColor(color);
		SetSize(sf::Vector2f(text.getGlobalBounds().width + text.getGlobalBounds().left, text.getGlobalBounds().height + text.getGlobalBounds().top));
		needToUpdateText = false;
		Show();
	}
}

bool Label::HandleEvent(sf::Event event) { return false; }

void Label::SetString(const sf::String &string) {
	this->string = string;
	needToUpdateText = true;
}

void Label::SetFont(const sf::Font &font) {
	this->font = &font;
	needToUpdateText = true;
}

void Label::SetColor(const sf::Color &color) {
	this->color = color;
	needToUpdateText = true;
}

void Label::SetFontSize(unsigned size) {
	this->size = size;
	needToUpdateText = true;
}