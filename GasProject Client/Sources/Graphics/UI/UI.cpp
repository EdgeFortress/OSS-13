#include "UI.hpp"

#include <SFML/Graphics.hpp>

#include "Client.hpp"

using std::string; 

UI::UI() {
    if (!font.loadFromFile("Arialuni.ttf"))
        CC::log << "Font load error!" << std::endl;
    //authUI = uptr<AuthUI>(new AuthUI(this)),
    //gameProcessUI = uptr<GameProcessUI>(new GameProcessUI(this));
    //gamelistUI = uptr<GameListUI>(new GameListUI(this));

    background.loadFromFile("Images/MenuBackground.jpg");
    background_sprite.setTexture(background);

	curUIModule.reset(new AuthUI(this));
}

void UI::Resize(int width, int height) {
    float scaleX = float(width) / background.getSize().x;
    float scaleY = float(height) / background.getSize().y;
    background_sprite.setScale(scaleX, scaleY);

	curUIModule->Resize(width, height);

	size = sf::Vector2i(width, height);
}

void UI::HandleEvent(sf::Event event) { 
	curUIModule->HandleEvent(event); 
}
void UI::Update(sf::Time timeElapsed) { 
	if (newUIModule) {
		curUIModule.reset(newUIModule);
		newUIModule = nullptr;
		curUIModule->Resize(size.x, size.y);
		curUIModule->Initialize();
	}
	curUIModule->Update(timeElapsed);
}
void UI::Draw(sf::RenderWindow *render_window) { 
	curUIModule->Draw(render_window); 
}

void UI::DrawMenuBackground(sf::RenderWindow *render_window) {
    render_window->draw(background_sprite);
}

void UI::Lock() {
    mutex.lock();
};

void UI::Unlock() {
    mutex.unlock();
}

const sf::Font &UI::GetFont() const { return font; }
UIModule *UI::GetCurrentUIModule() { return curUIModule.get(); }

