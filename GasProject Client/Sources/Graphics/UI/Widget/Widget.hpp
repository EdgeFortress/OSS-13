#pragma once

#include <SFML/Graphics.hpp>

class Container;

class Widget : private sf::Transformable, public sf::Drawable {
private:
	sf::Vector2f size;
    sf::Sprite bufferSprite;

    Widget *parent;
	// Only container needs this function
	void setParent(Widget *widget);

    bool hiding;

protected:
	sf::RenderTexture buffer;
	// method for drawing to buffer
	virtual void draw() const = 0;

public:
	explicit Widget(sf::Vector2f size = sf::Vector2f());

    // draw method for Drawable inheritance
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	// public method for Draw widget buffer to target
	void Draw(sf::RenderTarget &target) const;

	virtual void Update(sf::Time timeElapsed) = 0;
	virtual bool HandleEvent(sf::Event event) = 0;

	void Hide();
	void Show();
	bool Visibility();

	void SetPosition(const sf::Vector2f);
	void SetPosition(const float x, const float y);
	sf::Vector2f GetPosition() const;
    sf::Vector2f GetAbsPosition() const;

	void SetSize(const sf::Vector2f &);
	sf::Vector2f GetSize() const;

	friend Container;
};