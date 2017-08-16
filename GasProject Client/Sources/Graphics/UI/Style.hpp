#pragma once

#include <SFML/Graphics.hpp>

#include "Shared/Types.hpp"

struct Style {
    const sf::Font *font;
    sf::Color textColor;
    uint fontSize;

    sf::Color backgroundColor;
    sf::Color borderColor;

    //sf::Color underCursor_textColor;
    //sf::Color underCursor_backgroundColor;
    //sf::Color underCursor_borderColor;

    bool updated;

    Style();
    ~Style() = default;
    Style(const Style &) = default;
    Style &operator=(const Style &) = default;
    Style(Style &&) = default;
    Style &operator=(Style &&) = default;
};