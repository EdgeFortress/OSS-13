#include "Label.hpp"

#include "Client.hpp"

Label::Label(const sf::String &string, const sf::Font &font, const unsigned &size, const sf::Color &color) :
	text(string, font, size)
{
	text.setFillColor(color);
	text.setOutlineColor(color);
	CC::log << int(text.getLocalBounds().width) << int(text.getLocalBounds().height) << endl;
	//SetSize(sf::Vector2f(text.getLocalBounds().width, text.getLocalBounds().height));
	SetSize(sf::Vector2f(100, 100));
}

void Label::draw() const {
	sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
	buffer.clear(sf::Color::Transparent);
	buffer.draw(text);
	buffer.display();
}

void Label::Update() { }

void Label::SetPosition(const sf::Vector2f pos) {
	text.setPosition(pos);
	Widget::SetPosition(pos);
}
void Label::SetPosition(const float x, const float y) {
	text.setPosition(x, y);
	Widget::SetPosition(x, y);
}