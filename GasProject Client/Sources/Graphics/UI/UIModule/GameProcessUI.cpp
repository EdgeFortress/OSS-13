#include "GameProcessUI.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid/TileGrid.hpp"
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
    infoLabel(ui->GetFont()), chat(ui->GetFont())
{ 
    generateFunctionWindow();

    //Hide();
}

void GameProcessUI::generateFunctionWindow() {
	functionWindow = new Container();
	functionWindow->SetBackground(sf::Color(69, 69, 69));
	widgets.push_back(uptr<Container>(functionWindow));
}

void GameProcessUI::Initialize() { }

void GameProcessUI::Resize(const int width, const int height) { 
    infoLabel.CountPosition(width, height);
    chat.Resize(width, height);
    functionWindow->SetPosition(CC::Get()->GetWindow()->GetTileGrid()->GetTileSize() * float(Global::FOV), 0);
    functionWindow->SetSize(sf::Vector2f(width - functionWindow->GetAbsPosition().x, chat.GetYPos()));
    //TileGrid *tileGrid = CC::Get()->GetWindow()->GetTileGrid();
}

void GameProcessUI::Draw(sf::RenderWindow* renderWindow) { 
	CC::Get()->GetWindow()->GetTileGrid()->Draw(renderWindow);
	for (auto &widget : widgets) widget->Draw(*renderWindow); 
	infoLabel.Draw(renderWindow);
	chat.Draw(renderWindow);
}

void GameProcessUI::Update(sf::Time timeElapsed) {
    chat.Update(timeElapsed);
    for (auto &widget : widgets) 
        widget->Update(timeElapsed);
	CC::Get()->GetWindow()->GetTileGrid()->Update(timeElapsed);
}

void GameProcessUI::HandleEvent(sf::Event event) { 
	for (auto &widget : widgets) widget->HandleEvent(event); 

	static bool playing = true;
	bool chatting = chat.GetState();

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
		playing = !playing, chatting = !chatting, chat.SetState(chatting);
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x > chat.GetXPos() && event.mouseButton.y > chat.GetYPos())
		playing = false, chatting = true, chat.SetState(chatting);
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x < chat.GetXPos())
		playing = true, chatting = false, chat.SetState(chatting);

	if (playing) CC::Get()->GetWindow()->GetTileGrid()->HandleEvent(event);

	if (event.type == sf::Event::MouseMoved) {
		Object *obj = CC::Get()->GetWindow()->GetTileGrid()->GetObjectByPixel(event.mouseMove.x, event.mouseMove.y);
		if (obj != nullptr) infoLabel.SetText(obj->GetName());
		else infoLabel.SetText("");
	}

	if (chatting) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Return)
				chat.Send();
			if (event.key.code == sf::Keyboard::BackSpace)
				chat.DeleteSymbol();
			if (event.key.code == sf::Keyboard::Left)
				chat.MoveLeft();
			if (event.key.code == sf::Keyboard::Right)
				chat.MoveRight();
			if (event.key.code == sf::Keyboard::Up)
				chat.ScrollUp();
			if (event.key.code == sf::Keyboard::Down)
				chat.ScrollDown();
		}
		if (event.type == sf::Event::TextEntered) {
			wchar_t c = wchar_t(event.text.unicode);
			if (c != '\r' && c != '\t' && c != '\b')
				chat.SetSymbol(c);
		}
	}
}

InfoLabel *GameProcessUI::GetInfoLabel() { return &infoLabel; }
Chat *GameProcessUI::GetChat() { return &chat; }