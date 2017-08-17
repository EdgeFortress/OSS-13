#pragma once

#include <functional>

#include "Widget.hpp"

class Button : public Widget {
public:
    Button(sf::Vector2f size = sf::Vector2f());
    Button(const sf::String &, const sf::Vector2f &size, std::function<void()>);

    virtual void Update(sf::Time timeElapsed) final;
    virtual bool HandleEvent(sf::Event event) final;

    void SetString(const sf::String &string);
    void SetUnderCursorStyle(const Style &);
    void SetUnderCursorStyle(Style &&);

protected:
    virtual void draw() const final;

private:
    sf::Text text;

	// underCursor options
    Style underCursor_style;
    bool underCursor;
    //bool cursorDetected;

    std::function<void()> onPressFunc;
};