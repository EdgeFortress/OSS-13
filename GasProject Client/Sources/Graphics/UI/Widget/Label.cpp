#include "Label.hpp"

Label::Label(const sf::String &string, const sf::Font &font) :
	text(string, font)
{

}

void Label::Draw(sf::RenderTarget &target) const {
	target.draw(text);
}

void Label::Update() { }