#include "GameProcessUI.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid.hpp"
#include "Graphics/UI/Widget/ContextMenu.hpp"
#include "../UI.hpp"

GameProcessUI::GameProcessUI(UI *ui) : UIModule(ui),
    infoLabel(new InfoLabel(ui->GetFont()))
{ 
	generateFunctionWindow();

	tileGrid = new TileGrid();
	widgets.push_back(uptr<TileGrid>(tileGrid));

	chat = new Chat(ui->GetFont());
	widgets.push_back(uptr<Chat>(chat));

    contextMenu = new ContextMenu();
    contextMenu->AddRow(ContextMenuRow(ContextMenuRow::Type::FUNCTION, L"Test 1"));
    contextMenu->AddRow(ContextMenuRow(ContextMenuRow::Type::FUNCTION, L"Test 2"));
    contextMenu->AddRow(ContextMenuRow(ContextMenuRow::Type::FUNCTION, L"Тест 3"));
	contextMenu->GetStyle().backgroundColor = sf::Color(50, 50, 50);
	contextMenu->GetStyle().textColor = sf::Color(220, 220, 220);
	contextMenu->GetStyle().fontSize = 11;
	contextMenu->GetUnderCursorStyle().backgroundColor = sf::Color(100, 100, 100);
    widgets.push_back(uptr<ContextMenu>(contextMenu));

	curInputWidget = tileGrid;
}

void GameProcessUI::Initialize() { }

void GameProcessUI::Resize(const int width, const int height) {
    tileGrid->SetSize(uf::vec2i(width, height));
    infoLabel->CountPosition(width, height);
    chat->SetSize(uf::vec2i(width, height));
    functionWindow->SetPosition(tileGrid->GetTileSize() * float(Global::FOV), 0);
    functionWindow->SetSize({width - functionWindow->GetAbsPosition().x, chat->GetPosition().y});
}

void GameProcessUI::Draw(sf::RenderWindow *renderWindow) {
    for (auto &widget : widgets) widget->Draw(*renderWindow);
    infoLabel->Draw(renderWindow);
}

void GameProcessUI::Update(sf::Time timeElapsed) {
    for (auto &widget : widgets)
        widget->Update(timeElapsed);
}

void GameProcessUI::HandleEvent(sf::Event event) {
    contextMenu->HandleEvent(event);

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
        default:
            break;
        }
    default:
        break;
    }
    case sf::Event::MouseMoved: {
        Object *obj = tileGrid->GetObjectByPixel({event.mouseMove.x, event.mouseMove.y});
        if (obj != nullptr) infoLabel->SetText(obj->GetName());
        else infoLabel->SetText("");
        break;
    }
    }

    UIModule::HandleEvent(event);
}

InfoLabel *GameProcessUI::GetInfoLabel() const { return infoLabel.get(); }
Chat *GameProcessUI::GetChat() const { return chat; }
TileGrid *GameProcessUI::GetTileGrid() const { return tileGrid; }

void GameProcessUI::generateFunctionWindow() {
	functionWindow = new Container();
    functionWindow->GetStyle().backgroundColor = sf::Color(69, 69, 69);
	widgets.push_back(uptr<Container>(functionWindow));
}

InfoLabel::InfoLabel(const sf::Font &font) {
    rectangle.setSize(sf::Vector2f(100, 15));
    rectangle.setFillColor(sf::Color(31, 31, 31));

    text.setString("Test Label");
    text.setFont(font);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color(255, 69, 0));
}

void InfoLabel::Draw(sf::RenderWindow *renderWindow) const {
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