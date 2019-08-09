#pragma once

#include <Graphics/UI/Widget/Widget.hpp>
#include <Graphics/UI/Style.hpp>

class CustomWidget : public Widget, private sf::Transformable, public sf::Drawable {
	friend Container;

public:
	explicit CustomWidget(uf::vec2i size = {});

	// draw method for Drawable inheritance
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	virtual void Draw(sf::RenderTarget &target) const final;
	virtual void Update(sf::Time timeElapsed) = 0;
	virtual bool HandleEvent(sf::Event event) = 0;

	virtual void Hide() final;
	virtual void Show() final;

	virtual bool SetActive(bool);

	void SetStyle(const Style &);
	void SetStyle(Style &&);

	virtual void SetPosition(const uf::vec2i &);
	virtual void SetPosition(const float x, const float y);
	virtual void SetSize(const uf::vec2i &);

	virtual uf::vec2i GetPosition() const final;
	virtual uf::vec2i GetAbsPosition() const final;
	virtual uf::vec2i GetSize() const final;
	virtual bool IsVisible() const final;

	bool IsActive() const;
	// Get style to change it. Set style.updated to true!
	Style &GetStyle();

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
