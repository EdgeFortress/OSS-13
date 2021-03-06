#pragma once

#include <SFML/Graphics.hpp>

#include <Shared/Types.hpp>

class Container;

class Widget {
public:
	explicit Widget(uf::vec2i size = {});

	Widget(const Widget &) = default;
	Widget(Widget &&) = default;
	Widget &operator=(const Widget &) = default;
	Widget &operator=(Widget &&) = default;
	virtual ~Widget() = default;

	virtual void Draw(sf::RenderTarget &target) const = 0;
	virtual void Update(sf::Time timeElapsed) = 0;
	virtual bool HandleEvent(sf::Event event) = 0;

	virtual void Hide() = 0;
	virtual void Show() = 0;
	void Close();

	virtual void SetPosition(const uf::vec2i &) = 0;
	virtual void SetPosition(const float x, const float y) = 0;
	virtual void SetSize(const uf::vec2i &) = 0;

	virtual Widget *GetParent() const = 0;
	virtual uf::vec2i GetPosition() const = 0;
	virtual uf::vec2i GetAbsolutePosition() const = 0;
	virtual uf::vec2i GetSize() const = 0;
	virtual uf::vec2i GetAbsoluteSize() const = 0;
	virtual uf::vec2f GetScale() const = 0;
    virtual bool IsVisible() const = 0;
	bool IsClosed();

private:
	bool isClosed;
};
