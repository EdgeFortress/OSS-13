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

UI::UI(sf::RenderWindow *rendWindow) :
    rendWindow(rendWindow),
    authUI(new AuthUI(this)),
    gamelistUI(new GameListUI(this)),
    gameProcessUI(new GameProcessUI(this))
{
    
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

void UI::Lock() {
    UImutex.lock();
};

void UI::Unlock() {
    UImutex.unlock();
}

