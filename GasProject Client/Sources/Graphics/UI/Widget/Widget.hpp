#pragma once

#include <SFML/Graphics.hpp>

#include "Graphics/UI/Style.hpp"

class Container;

class Widget : private sf::Transformable, public sf::Drawable {
public:
    explicit Widget(uf::vec2i size = {});
	virtual ~Widget() = default;

    // draw method for Drawable inheritance
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	// public method for Draw widget buffer to target
	void Draw(sf::RenderTarget &target) const;
	virtual void Update(sf::Time timeElapsed) = 0;
	virtual bool HandleEvent(sf::Event event) = 0;

	void Hide();
	void Show();

    void SetStyle(const Style &);
    void SetStyle(Style &&);
	void SetPosition(const uf::vec2i &);
	void SetPosition(const float x, const float y);
	virtual void SetSize(const uf::vec2i &);
	virtual bool SetActive(bool);

    // Get style to change it. Set style.updated to true!
    Style &GetStyle();
	uf::vec2i GetPosition() const;
	uf::vec2i GetAbsPosition() const;
	uf::vec2i GetSize() const;
	bool IsActive() const;
    bool IsVisible() const;

	friend Container;

protected:
    Style style;
    bool canBeActive;

    mutable sf::RenderTexture buffer;
    // method for drawing to buffer
    virtual void draw() const = 0;

private:
    uf::vec2i size;
    mutable sf::Sprite bufferSprite;

    Widget *parent;
    // Only container needs this function
    void setParent(Widget *widget);

    bool hiding;
    bool active;
};