#pragma once

#include <SFML/Graphics.hpp>

class Container;

class Widget : private sf::Transformable, public sf::Drawable {
private:
	sf::Vector2f size;
    mutable sf::Sprite bufferSprite;

    Widget *parent;
	// Only container needs this function
	void setParent(Widget *widget);

    bool hiding;
	bool active;

protected:
	bool canBeActive;

	mutable sf::RenderTexture buffer;
	// method for drawing to buffer
	virtual void draw() const = 0;

public:
	explicit Widget(sf::Vector2f size = sf::Vector2f());
	virtual ~Widget() = default;

    // draw method for Drawable inheritance
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	// public method for Draw widget buffer to target
	void Draw(sf::RenderTarget &target) const;
	virtual void Update(sf::Time timeElapsed) = 0;
	virtual bool HandleEvent(sf::Event event) = 0;

	void Hide();
	void Show();
	bool IsVisible() const;

	void SetPosition(const sf::Vector2f);
	void SetPosition(const float x, const float y);
	virtual void SetSize(const sf::Vector2f &);
	virtual bool SetActive(bool);

	sf::Vector2f GetPosition() const;
	sf::Vector2f GetAbsPosition() const;
	sf::Vector2f GetSize() const;
	bool IsActive() const;

	friend Container;
};