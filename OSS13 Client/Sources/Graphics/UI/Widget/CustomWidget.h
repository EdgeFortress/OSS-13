#pragma once

#include <Graphics/UI/Widget/Widget.hpp>
#include <Graphics/UI/Style.hpp>

class CustomWidget : public Widget, private sf::Transformable, public sf::Drawable {
	friend Container;

public:
	explicit CustomWidget(uf::vec2i size = {});

	CustomWidget(const CustomWidget &) = default;
	CustomWidget(CustomWidget &&) = default;
	CustomWidget &operator=(const CustomWidget &) = default;
	CustomWidget &operator=(CustomWidget &&) = default;
	virtual ~CustomWidget() = default;

	// draw method for Drawable inheritance
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	virtual void Draw(sf::RenderTarget &target) const final;
	virtual void Update(sf::Time timeElapsed) = 0;
	virtual bool HandleEvent(sf::Event event) override;

	virtual bool OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position);
	virtual bool OnMouseButtonReleased(sf::Mouse::Button button, uf::vec2i position);
	virtual bool OnMouseMoved(uf::vec2i position);
	virtual bool OnMouseLeft();
	virtual bool OnMouseWheelScrolled(float delta, uf::vec2i position);
	virtual bool OnKeyPressed(sf::Event::KeyEvent keyEvent);
	virtual bool OnKeyReleased(sf::Event::KeyEvent keyEvent);
	virtual bool OnTextEntered(uint32_t unicodeChar);

	virtual void Hide() final;
	virtual void Show() final;

	virtual bool SetActive(bool);

	void SetStyle(const Style &);
	void SetStyle(Style &&);

	virtual void SetPosition(const uf::vec2i &);
	virtual void SetPosition(const float x, const float y);
	virtual void SetSize(const uf::vec2i &);
	virtual void SetScale(const uf::vec2f &scale);

	virtual Widget *GetParent() const final;
	virtual uf::vec2i GetPosition() const final;
	virtual uf::vec2i GetAbsolutePosition() const final;
	virtual uf::vec2i GetSize() const final;
	virtual uf::vec2i GetAbsoluteSize() const final;
	virtual uf::vec2f GetScale() const final;
	virtual bool IsVisible() const final;

	bool IsActive() const;
	// Get style to change it. Set style.updated to true!
	Style &GetStyle();

	mutable sf::RenderTexture buffer;

protected:
	Style style;
	bool canBeActive;

	
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
