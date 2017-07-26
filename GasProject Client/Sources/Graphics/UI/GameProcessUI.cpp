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

void InfoLabel::Draw(RenderWindow *renderWindow) {
    renderWindow->draw(rectangle);

    renderWindow->draw(text);
}

void InfoLabel::CountPosition(int width, int height) {
    rectangle.setPosition(0, height - rectangle.getSize().y);
    text.setPosition(5, height - rectangle.getSize().y - 2);
}

void InfoLabel::SetText(const string &s) {
    text.setString(s);
}

void GameProcessUI::generateFunctionWindow() {
    functionWindow = new Container(sf::Vector2f(0, 0));
    functionWindow->SetBackground(sf::Color(69, 69, 69));
    widgets.push_back(uptr<Container>(functionWindow));
}

GameProcessUI::GameProcessUI(UI *ui) : UIModule(ui),
    infoLabel(ui->GetFont()), chat(ui->GetFont())
{ 
    generateFunctionWindow();

    //Hide();
}

void GameProcessUI::Resize(const int width, const int height) { 
    infoLabel.CountPosition(width, height);
    chat.Resize(width, height);
    functionWindow->SetPosition(CC::Get()->GetWindow()->GetTileGrid()->GetTileSize() * float(Global::FOV), 0);
    functionWindow->SetSize(sf::Vector2f(width - functionWindow->GetAbsPosition().x, chat.GetYPos()));
    //TileGrid *tileGrid = CC::Get()->GetWindow()->GetTileGrid();
}

void GameProcessUI::Draw(sf::RenderWindow *renderWindow, sf::Time timeElapsed) {
    infoLabel.Draw(renderWindow);
    chat.Draw(renderWindow, timeElapsed);
}

void GameProcessUI::Update(sf::Time timeElapsed) {
    chat.Update(timeElapsed);
    for (auto &widget : widgets) 
        widget->Update(timeElapsed);
}

void GameProcessUI::Show() {}

void GameProcessUI::Hide() {}