#pragma once

#include <functional>

#include "Widget.hpp"

class Button : public Widget {
private:
    sf::Text text;

	// text options
	sf::String string;
	sf::Color textColor;
    const sf::Font *font;
	unsigned fontSize;
	bool needToUpdateText;

	sf::Color backgroundColor;

	// underCursor options
	bool underCursorColorChanging;
	sf::Color underCursor_textColor;
	sf::Color underCursor_backgroundColor;
    sf::Time underCursor_time;
    bool underCursor;
	const sf::Time underCursor_newEventWaitingTime = sf::seconds(0.1f);

    std::function<void()> onPressFunc;

protected:
    virtual void draw() const final;

public:
	Button(sf::Vector2f size = sf::Vector2f());
    Button(const sf::String &, sf::Vector2f &size, const sf::Color &textColor, const sf::Font &, unsigned fontSize, std::function<void()>);

    virtual void Update(sf::Time timeElapsed) final;
    virtual bool HandleEvent(sf::Event event) final;

	void SetString(const sf::String &string);
	void SetFont(const sf::Font &font);
	void SetTextColor(const sf::Color &color);
	void SetFontSize(unsigned size);
};