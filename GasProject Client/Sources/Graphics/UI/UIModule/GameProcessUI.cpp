#include "GameProcessUI.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid.hpp"
#include "Graphics/UI/Widget/ContextMenu.hpp"
#include "../UI.hpp"
#include "Network.hpp"

GameProcessUI::GameProcessUI(UI *ui) : UIModule(ui),
    infoLabel(new InfoLabel(ui->GetFont()))
{ 
	generateFunctionWindow();

	tileGrid = new TileGrid();
	widgets.push_back(uptr<TileGrid>(tileGrid));

    container = new Container();
    container->GetStyle().backgroundColor = sf::Color::Transparent;
    container->SetPosition(0, 0);
    widgets.push_back(uptr<Container>(container));

    entry = new Entry(sf::Vector2f(0, 0));
    entry->SetOnEnterFunc(std::bind(&GameProcessUI::send, this));
    entry->GetStyle().backgroundColor = sf::Color(31, 31, 31);
    entry->GetStyle().textColor = sf::Color(193, 205, 205);
    entry->GetStyle().fontSize = 18;
    container->AddItem(entry, sf::Vector2f(0, 0));

    formattedTextField = new FormattedTextField(sf::Vector2f(0, 0));
    formattedTextField->GetStyle().backgroundColor = sf::Color(60, 60, 60);
    formattedTextField->GetStyle().textColor = sf::Color(193, 205, 205);
    formattedTextField->GetStyle().fontSize = 18;
    container->AddItem(formattedTextField, sf::Vector2f(0, 0));

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

void GameProcessUI::send() {
    if (!entry->Empty())
        Connection::commandQueue.Push(new SendChatMessageClientCommand(entry->GetText()));
}

void GameProcessUI::Receive(const std::string &message) {
    formattedTextField->AddInscription(message);
}


void GameProcessUI::Resize(const int width, const int height) {
    tileGrid->SetSize(uf::vec2i(width, height));
    infoLabel->CountPosition(width, height);

    TileGrid *tileGrid = reinterpret_cast<GameProcessUI *>(CC::Get()->GetUI()->GetCurrentUIModule())->GetTileGrid();

    container->SetSize(sf::Vector2f(width - tileGrid->GetTileSize() * float(Global::FOV), height * 0.5f));
    entry->SetSize({ container->GetSize().x, int(container->GetSize().y * 0.1f) });
    formattedTextField->SetSize(uf::vec2i(container->GetSize().x, container->GetSize().y - entry->GetSize().y));

    entry->SetPosition(0, float(formattedTextField->GetSize().y));
    container->SetPosition({ width - container->GetSize().x, height - entry->GetSize().y - formattedTextField->GetSize().y });

    functionWindow->SetPosition(tileGrid->GetTileSize() * float(Global::FOV), 0);
    functionWindow->SetSize({width - functionWindow->GetAbsPosition().x, container->GetPosition().y});
}

void GameProcessUI::Draw(sf::RenderWindow *renderWindow) {
    for (auto &widget : widgets) widget->Draw(*renderWindow);
    infoLabel->Draw(renderWindow);
}

void GameProcessUI::Update(sf::Time timeElapsed) {
    for (auto &widget : widgets)
        widget->Update(timeElapsed);

    Object *objectUnderCursor = tileGrid->GetObjectUnderCursor();
    if (!objectUnderCursor) infoLabel->SetText("");
    else infoLabel->SetText(objectUnderCursor->GetName());
}

void GameProcessUI::HandleEvent(sf::Event event) {
    contextMenu->HandleEvent(event);

    switch (event.type) {
    case sf::Event::KeyPressed: {
        switch (event.key.code) {
        case sf::Keyboard::Tab: {
            if (container->IsActive()) {
                container->SetActive(false);
                tileGrid->SetActive(true);
                curInputWidget = tileGrid;
            } else {
                container->SetActive(true);
                tileGrid->SetActive(false);
                curInputWidget = container;
            }
            return;
        }
        default:
            break;
        }
    default:
        break;
    }
    }

    UIModule::HandleEvent(event);
}

InfoLabel *GameProcessUI::GetInfoLabel() const { return infoLabel.get(); }
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