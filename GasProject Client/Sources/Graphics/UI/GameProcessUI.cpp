#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid/TileGrid.hpp"
#include "GameProcessUI.hpp"

InfoLabel::InfoLabel() {
    rectangle.setSize(sf::Vector2f(100, 15));
    text.setString("Test");
}

void InfoLabel::Draw(RenderWindow *window) {
    window->draw(rectangle);
}

void InfoLabel::CountPosition(int width, int height) {
    rectangle.setPosition(0, height - rectangle.getSize().y);
}

void InfoLabel::SetText(string s) {
    text.setString(s);
}

void GameProcessUI::drawInfoPanel(sf::RenderWindow *renderWindow) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow);
    //CC::log << mousePosition.x << " " << mousePosition.y << endl;
    Tile *tile = CC::Get()->GetWindow()->GetTileGrid()->GetTileByPixel(mousePosition.x, mousePosition.y);
    if(tile != NULL) {
        CC::log << tile->GetNumObj() << endl;
    }
}

GameProcessUI::GameProcessUI(UI *ui) :
    UIModule(ui)
{ }

void GameProcessUI::Resize(int width, int height) { 
    infoLabel.CountPosition(width, height);
}

void GameProcessUI::Draw(sf::RenderWindow *renderWindow) {
    infoLabel.Draw(renderWindow);
}
