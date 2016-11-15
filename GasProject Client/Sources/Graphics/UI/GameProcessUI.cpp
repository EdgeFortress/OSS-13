#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid/TileGrid.hpp"
#include "GameProcessUI.hpp"

InfoLabel::InfoLabel(const sf::Font &font) {
    rectangle.setSize(sf::Vector2f(100, 15));
    rectangle.setFillColor(sf::Color(31, 31, 31));

    text.setString("Test Label");
    text.setFont(font);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color(255, 69, 0));
}

void InfoLabel::Draw(RenderWindow *window) {
    window->draw(rectangle);
    window->draw(text);
}

void InfoLabel::CountPosition(int width, int height) {
    rectangle.setPosition(0, height - rectangle.getSize().y);
    text.setPosition(5, height - rectangle.getSize().y - 2);
}

void InfoLabel::SetText(string s) {
    text.setString(s);
}

//void GameProcessUI::drawInfoPanel(sf::RenderWindow *renderWindow) {
//    sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow);
//    //CC::log << mousePosition.x << " " << mousePosition.y << endl;
//    Tile *tile = CC::Get()->GetWindow()->GetTileGrid()->GetTileByPixel(mousePosition.x, mousePosition.y);
//    if(tile != NULL) {
//        CC::log << tile->GetNumObj() << endl;
//    }
//}

GameProcessUI::GameProcessUI(UI *ui) : UIModule(ui),
    infoLabel(ui->GetFont())
{ }

void GameProcessUI::Resize(int width, int height) { 
    infoLabel.CountPosition(width, height);
}

void GameProcessUI::Draw(sf::RenderWindow *renderWindow) {
    infoLabel.Draw(renderWindow);
}
