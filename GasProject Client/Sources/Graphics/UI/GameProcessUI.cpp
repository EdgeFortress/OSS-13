#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

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

void InfoLabel::SetText(string s) {
    text.setString(s);
}

void GameProcessUI::generateChatWindow() {
    chatWindow = sfg::Window::Create();
    chatWindow->SetStyle(sfg::Window::Style::TOPLEVEL ^ sfg::Window::Style::RESIZE ^ sfg::Window::Style::TITLEBAR);

    ui->GetDesktop()->Add(chatWindow);
}

GameProcessUI::GameProcessUI(UI *ui) : UIModule(ui),
    infoLabel(ui->GetFont())
{ 
    generateChatWindow();

    Hide();
}

void GameProcessUI::Resize(const int width, const int height) { 
    infoLabel.CountPosition(width, height);
    chatWindow->SetAllocation(sf::FloatRect(float(height), 0, float(width - height), float(height)));
}

void GameProcessUI::Draw(sf::RenderWindow *renderWindow) {
    infoLabel.Draw(renderWindow);
}
void GameProcessUI::Show() {
    chatWindow->Show(true);
}

void GameProcessUI::Hide() {
    chatWindow->Show(false);
}