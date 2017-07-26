#include <string>
#include <cstring>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Client.hpp"

#include "Graphics/UI/UI.hpp"
#include "Graphics/UI/AuthUI.hpp"
#include "Graphics/UI/GameListUI.hpp"
#include "Graphics/UI/GameProcessUI.hpp"

using std::string; 

UIModule::UIModule(UI *ui) : ui(ui) {

}

UI::UI() {
    if (!font.loadFromFile("Arialuni.ttf"))
    //if (!font.loadFromFile("agency.ttf"))
        CC::log << "Font load error!" << endl;
    //CC::log << "Font loaded!" << endl;
    authUI = uptr<AuthUI>(new AuthUI(this)),
    gameProcessUI = uptr<GameProcessUI>(new GameProcessUI(this));
    gamelistUI = uptr<GameListUI>(new GameListUI(this));

    background.loadFromFile("Images/MenuBackground.jpg");
    background_sprite.setTexture(background);
}

void UI::Resize(int width, int height) {
    float scaleX = float(width) / background.getSize().x;
    float scaleY = float(height) / background.getSize().y;
    background_sprite.setScale(scaleX, scaleY);

    authUI->Resize(width, height);
    gamelistUI->Resize(width, height);
    gameProcessUI->Resize(width, height);
}

void UI::HandleEvent(sf::Event event) {}

void UI::Update(sf::Time timeElapsed) {}

void UI::Draw(sf::RenderWindow *render_window) {}

void UI::DrawMenuBackground(sf::RenderWindow *render_window) {
    render_window->draw(background_sprite);
}

void UI::Lock() {
    UImutex.lock();
};

void UI::Unlock() {
    UImutex.unlock();
}

