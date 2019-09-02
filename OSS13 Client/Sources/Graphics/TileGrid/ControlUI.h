#pragma once

#include <Graphics/Sprite.hpp>
#include <Graphics/UI/Widget/CustomWidget.h>

#include <Shared/Network/Protocol/ServerToClient/ControlUIData.h>

class ControlUIElement : public CustomWidget {
public:
	ControlUIElement();

	void Update(sf::Time timeElapsed) final;
	bool HandleEvent(sf::Event event) final;

	void SetSprites(const std::vector<uint> &spritesIds);

protected:
	void draw() const final;

private:
	std::vector<Sprite> sprites;
};

class ControlUI : public CustomWidget {
public:
	ControlUI();

	void Update(sf::Time timeElapsed) final;
	bool HandleEvent(sf::Event event) final;

	void AdjustSize(uf::vec2i size);

	void UpdateElement(const network::protocol::ControlUIData &data);

protected:
	void draw() const final;

private:
	std::unordered_map<std::string, ControlUIElement> elements;
	double scale;
};
