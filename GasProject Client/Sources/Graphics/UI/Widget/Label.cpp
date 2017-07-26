#include "Label.hpp"

Label::Label(const sf::String &string, const sf::Font &font, const unsigned &size, const sf::Color &color)
	: text(string, font, size)
{
	text.setFillColor(color);
	text.setOutlineColor(color);
    SetSize(sf::Vector2f(text.getGlobalBounds().width + text.getGlobalBounds().left, text.getGlobalBounds().height + text.getGlobalBounds().top));
}

Label::Label(const sf::String &string, const unsigned &size, const sf::Color &color) : name(string) {
    text.setFillColor(color);
    text.setOutlineColor(color);
    SetSize(sf::Vector2f(text.getGlobalBounds().width + text.getGlobalBounds().left, text.getGlobalBounds().height + text.getGlobalBounds().top));
}

void Label::draw() const {
	sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
	buffer.clear(sf::Color::Transparent);
	buffer.draw(text);
	buffer.display();
}

void Label::Update(sf::Time timeElapsed) {}

void Label::SetPosition(const sf::Vector2f pos) {
	//text.setPosition(pos);
	Widget::SetPosition(pos);
}
void Label::SetPosition(const float x, const float y) {
	//text.setPosition(x, y);
	Widget::SetPosition(x, y);
}

void Label::SetFont(const sf::Font &font) {
    this->font = font;
    text = sf::Text(name, font, 16);
}