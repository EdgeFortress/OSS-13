#include "GameProcessUI.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid.hpp"
#include "../UI.hpp"

InfoLabel::InfoLabel(const sf::Font &font) {
    rectangle.setSize(sf::Vector2f(100, 15));
    rectangle.setFillColor(sf::Color(31, 31, 31));

    text.setString("Test Label");
    text.setFont(font);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color(255, 69, 0));
}

void InfoLabel::Draw(sf::RenderWindow *renderWindow) {
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

GameProcessUI::GameProcessUI(UI *ui) : UIModule(ui),
    infoLabel(ui->GetFont())
{ 
	generateFunctionWindow();

	tileGrid = new TileGrid();
	widgets.push_back(uptr<TileGrid>(tileGrid));

	chat = new Chat(ui->GetFont());
	widgets.push_back(uptr<Chat>(chat));

	curInputWidget = tileGrid;
}

void GameProcessUI::generateFunctionWindow() {
	functionWindow = new Container();
	functionWindow->SetBackground(sf::Color(69, 69, 69));
	widgets.push_back(uptr<Container>(functionWindow));
}

void GameProcessUI::Initialize() { }

void GameProcessUI::Resize(const int width, const int height) { 
	tileGrid->SetSize(uf::vec2i(width, height));
    infoLabel.CountPosition(width, height);
    chat->SetSize(uf::vec2i(width, height));
    functionWindow->SetPosition(tileGrid->GetTileSize() * float(Global::FOV), 0);
    functionWindow->SetSize(sf::Vector2f(width - functionWindow->GetAbsPosition().x, chat->GetPosition().y));
}

void GameProcessUI::Draw(sf::RenderWindow *renderWindow) { 
	for (auto &widget : widgets) widget->Draw(*renderWindow); 
	infoLabel.Draw(renderWindow);
}

void GameProcessUI::Update(sf::Time timeElapsed) {
    for (auto &widget : widgets) 
        widget->Update(timeElapsed);
}

void GameProcessUI::HandleEvent(sf::Event event) { 
	switch (event.type) {
	case sf::Event::KeyPressed: {
		switch (event.key.code) {
		case sf::Keyboard::Tab: {
			if (chat->IsActive()) {
				chat->SetActive(false);
				tileGrid->SetActive(true);
				curInputWidget = tileGrid;
			} else {
				chat->SetActive(true);
				tileGrid->SetActive(false);
				curInputWidget = chat;
			}
			return;
		}
		}
	}
	case sf::Event::MouseMoved: {
		Object *obj = tileGrid->GetObjectByPixel(event.mouseMove.x, event.mouseMove.y);
		if (obj != nullptr) infoLabel.SetText(obj->GetName());
		else infoLabel.SetText("");
		break;
	}
	}

	UIModule::HandleEvent(event);
}

InfoLabel *GameProcessUI::GetInfoLabel() { return &infoLabel; }
Chat *GameProcessUI::GetChat() { return chat; }
TileGrid *GameProcessUI::GetTileGrid() { return tileGrid; }