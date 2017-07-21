#pragma once

#include <SFML/Graphics.hpp>

class Widget : private sf::Transformable, public sf::Drawable {
private:
	sf::Vector2f size;
    sf::Sprite bufferSprite;

    Widget *parent;

    bool hiding;

protected:
	sf::RenderTexture buffer;
	// method for drawing to buffer
	virtual void draw() const = 0;

public:
	explicit Widget(const sf::Vector2f &size = sf::Vector2f());

    virtual void Hide();
    virtual void Show();
    // draw method for Drawable inheritance
    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;

	// public method for Draw widget buffer to target
	void Draw(sf::RenderTarget &target) const;
	virtual void Update(sf::Time timeElapsed) = 0;
    virtual void HandleEvent(sf::Event event) {}

	virtual void SetPosition(const sf::Vector2f);
	virtual void SetPosition(const float x, const float y);
	virtual const sf::Vector2f GetPosition() const;
    virtual const sf::Vector2f GetAbsPosition() const;

	virtual void SetSize(const sf::Vector2f &);
	const virtual sf::Vector2f GetSize() const;

    virtual void SetParent(Widget *widget) { parent = widget; }
};