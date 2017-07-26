#pragma once

#include <list>

#include "Widget.hpp"
#include "Common/Useful.hpp"

class Container : public Widget {
private:
	sf::Vector2f size;
	//sf::RenderTexture buffer;
	//uptr<sf::Texture> buffer;
	std::list<std::pair<sf::Vector2f, uptr<Widget>>> items;
    sf::Color backgroundColor;

protected:
	virtual void draw() const override;

public:
	Container(const sf::Vector2f &size = sf::Vector2f());

    virtual void Hide();
    virtual void Show();

	virtual void Update(sf::Time timeElapsed) final;
    virtual void HandleEvent(sf::Event event) final;

	void AddItem(Widget *, sf::Vector2f position);
    void Clear();

	virtual void SetPosition(const sf::Vector2f) final;
	virtual void SetPosition(const float x, const float y) final;

    void SetBackground(sf::Color);

    virtual void SetFont(const sf::Font &font);
};