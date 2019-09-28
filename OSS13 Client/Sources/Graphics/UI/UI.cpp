#include "UI.hpp"

#include <SFML/Graphics.hpp>

#include <Client.hpp>
#include <Graphics/UI/Widget/Console.h>

using std::string; 

UI::UI() {
    if (!font.loadFromFile("Arialuni.ttf"))
        LOGE << "Failed to load font!";

    background.loadFromFile("Resources/Pictures/MenuBackground.jpg");
    background_sprite.setTexture(background);

    curUIModule = nullptr;
}

void UI::Resize(int width, int height) {
    float scaleX = float(width) / background.getSize().x;
    float scaleY = float(height) / background.getSize().y;
    background_sprite.setScale(scaleX, scaleY);

    if (curUIModule) curUIModule->Resize(width, height);

	size = sf::Vector2i(width, height);
}

void UI::HandleEvent(sf::Event event) { 
    if (curUIModule) curUIModule->HandleEvent(event); 
}

void UI::Update(sf::Time timeElapsed) { 
	if (!console)
		console = std::make_unique<Console>(); // TODO: move console from here
	console->Update(timeElapsed);

	if (newUIModule) {
		curUIModule.reset(newUIModule);
		newUIModule = nullptr;
		curUIModule->Resize(size.x, size.y);
		curUIModule->Initialize();
	}
    if (curUIModule) curUIModule->Update(timeElapsed);
}
void UI::Draw(sf::RenderWindow *render_window) { 
    if (curUIModule) curUIModule->Draw(render_window); 
}

void UI::DrawMenuBackground(sf::RenderWindow *render_window) {
    render_window->draw(background_sprite);
}

const sf::Font &UI::GetFont() const { return font; }
UIModule *UI::GetCurrentUIModule() { return curUIModule.get(); }

