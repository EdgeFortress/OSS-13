#include "ControlUI.h"

#include <Client.hpp>
#include <ResourceManager.hpp>

#include <Shared/Global.hpp>

ControlUIElement::ControlUIElement(const std::string &key) :
	key(key)
{ }

void ControlUIElement::Update(sf::Time timeElapsed) { }

bool ControlUIElement::OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) {
	position = uf::vec2f(position.x / GetScale().x, position.y / GetScale().y);

	if (!(position >= GetPosition() && position < GetPosition() + GetSize()))
		return false;

	for (auto &sprite: sprites) {
		if (!sprite.PixelTransparent(position - GetPosition())) {
			LOGI << "ControlUI element \"" + key + "\" is pressed!";
			return true;
		}
	}

	return false;
}

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
	for (auto &[key, element]: elements) 
		element->Update(timeElapsed);
}

bool ControlUI::OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) {
	position = uf::vec2f(position.x / GetScale().x, position.y / GetScale().y);
	for (auto &[key, element] : elements)
		if (element->OnMouseButtonPressed(button, position))
			return true;
	return false;
}

void ControlUI::AdjustSize(uf::vec2i size) {
	SetScale(size / float(GetSize().x));
}

void ControlUI::UpdateElement(const network::protocol::ControlUIData &data) {
	auto iter = elements.find(data.elementId);
	if (iter == elements.end()) {
		iter = elements.insert(iter, { data.elementId, std::make_unique<ControlUIElement>(data.elementId) });
	}
	auto &[id, element] = *iter;

	element->SetPosition(data.position);
	element->SetSize(uf::vec2i(Global::control_ui::ITEM_SIZE, Global::control_ui::ITEM_SIZE));
	element->SetSprites(data.spritesIds);
}

void ControlUI::draw() const {
	buffer.clear(sf::Color::Transparent);

	for (auto &[key, element] : elements) {
		buffer.draw(*element);
	}
	buffer.display();
}
