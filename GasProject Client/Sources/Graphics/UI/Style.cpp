#include "Style.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/UI/UI.hpp"

Style::Style() : 
    font(&CC::Get()->GetWindow()->GetUI()->GetFont()),
    backgroundColor(sf::Color::Transparent),
    textColor(sf::Color::Black),
    updated(false) 
{ }