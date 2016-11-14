#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid/TileGrid.hpp"
#include "GameProcessUI.hpp"

void GameProcessUI::drawInfoPanel(sf::RenderWindow * renderWindow)
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow);
    //CC::log << mousePosition.x << " " << mousePosition.y << endl;
    Tile *tile = CC::Get()->GetWindow()->GetTileGrid()->GetTileByPixel(mousePosition.x, mousePosition.y);
    if(tile != NULL){
        CC::log << tile->GetNumObj() << endl;
    }


}

GameProcessUI::GameProcessUI(UI * ui) :
    UIModule(ui),
    infoLabelBackground(sf::Vector2f(200, 30))
{ }

void GameProcessUI::Resize(int width, int height) 
{ }

void GameProcessUI::Draw(sf::RenderWindow* renderWindow)
{

    drawInfoPanel(renderWindow);
    

}
