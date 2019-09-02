#include "ControlUI.h"

#include <Client.hpp>
#include <ResourceManager.hpp>

#include <Shared/Global.hpp>

ControlUIElement::ControlUIElement()
{ }

void ControlUIElement::Update(sf::Time timeElapsed) { }
bool ControlUIElement::HandleEvent(sf::Event event) { return false; }

void ControlUIElement::SetSprites(const std::vector<uint> &spritesIds) {
	for (auto id : spritesIds) {
		auto sprite = CC::Get()->RM.CreateSprite(id);
		sprite.Resize(Global::control_ui::ITEM_SIZE);
		sprites.push_back(std::move(sprite));
	}
}

void ControlUIElement::draw() const {
	buffer.clear(sf::Color::Transparent);
	for (auto &sprite : sprites)
		sprite.Draw(&buffer, {0, 0});
	buffer.display();
}

ControlUI::ControlUI() {
	SetSize({Global::control_ui::SIDE_SIZE, Global::control_ui::SIDE_SIZE });
}

void ControlUI::Update(sf::Time timeElapsed) { 
	for (auto &[key, element]: elements) element.Update(timeElapsed);
}

bool ControlUI::HandleEvent(sf::Event event) {
	for (auto &[key, element] : elements) 
		if (element.HandleEvent(event))
			return true;
	return false;
}

void ControlUI::AdjustSize(uf::vec2i size) {
	SetScale(size / float(GetSize().x));
}

void ControlUI::UpdateElement(const network::protocol::ControlUIData &data) {
	auto &element = elements[data.elementId];
	element.SetPosition(data.position);
	element.SetSize(uf::vec2i(Global::control_ui::ITEM_SIZE, Global::control_ui::ITEM_SIZE));
	element.SetSprites(data.spritesIds);
}

void ControlUI::draw() const {
	buffer.clear(sf::Color::Transparent);

	for (auto &[key, element] : elements) {
		buffer.draw(element);
	}
	buffer.display();
}
