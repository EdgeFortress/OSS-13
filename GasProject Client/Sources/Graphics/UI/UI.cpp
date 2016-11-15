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

UI::UI() :
    authUI(new AuthUI(this)),
    gamelistUI(new GameListUI(this)),
    gameProcessUI(new GameProcessUI(this))
{
    if (!font.loadFromFile("agency.ttf"))
        CC::log << "Font load error!" << endl;
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

void UI::HandleEvent(sf::Event event) {
    desktop.HandleEvent(event);
}

void UI::Update(sf::Time timeElapsed) {
    desktop.Update(timeElapsed.asSeconds());
}

void UI::Draw(sf::RenderWindow *render_window) {
    m_sfgui.Display(*render_window);
}

void UI::DrawMenuBackground(sf::RenderWindow *render_window) {
    render_window->draw(background_sprite);
}

void UI::Lock() {
    UImutex.lock();
};

void UI::Unlock() {
    UImutex.unlock();
}

