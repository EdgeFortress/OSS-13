#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>

#include <Graphics/Sprite.hpp>
#include <Graphics/UI/Widget/CustomWidget.h>

#include <Shared/Network/Protocol/ServerToClient/ControlUIData.h>

class ControlUIElement : public CustomWidget {
public:
	explicit ControlUIElement(const std::string &key = {});

	ControlUIElement(const ControlUIElement &) = default;
	ControlUIElement &operator=(const ControlUIElement &) = default;
	ControlUIElement(ControlUIElement &&) = default;
	ControlUIElement &operator=(ControlUIElement &&) = default;

	void Update(sf::Time timeElapsed) final;

	bool OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) final;

	void SetSprites(const std::vector<uint> &spritesIds);

protected:
	void draw() const final;

private:
	void onClick();

private:
	std::string key;
	std::vector<Sprite> sprites;
	bool isClicked{false};
};

class ControlUI : public CustomWidget {
public:
	ControlUI();

	void Update(sf::Time timeElapsed) final;

	bool OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) final;

	void AdjustSize(uf::vec2i size);

	void UpdateElement(const network::protocol::ControlUIData &data);
	void Clear();

protected:
	void draw() const final;

private:
	std::unordered_map<std::string, uptr<ControlUIElement>> elements;
	mutable std::mutex elementsGuard;
	double scale;
};
