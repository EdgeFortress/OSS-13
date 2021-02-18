#include "GameProcessUI.hpp"

#include <memory>
#include <regex>

#include <imgui.h>
#include <imgui_stdlib.h>

#include <Client.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/TileGrid/TileGrid.hpp>
#include <Graphics/TileGrid/Object.hpp>
#include <Graphics/UI/UI.hpp>
#include <Graphics/UI/Widget/GameProcess/SpawnWindow.h>
#include <Network/Connection.h>

#include <Shared/Global.hpp>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>

using namespace network::protocol;

GameProcessUI::GameProcessUI(UI *ui) : UIModule(ui),
    infoLabel(new InfoLabel(ui->GetFont()))
{ 
	generateFunctionWindow();

	tileGrid = new TileGrid();
	widgets.push_back(uptr<TileGrid>(tileGrid));

    chat = new Chat(uf::vec2i{ 800, 500 }, *CC::Get()->GetWindow()->GetBrowserController());
    widgets.push_back(uptr<BrowserWidget>(chat));

	auto tileContextMenu = std::make_unique<TileContextMenu>();
	this->tileContextMenu = tileContextMenu.get();
	widgets.push_back(std::move(tileContextMenu));

	curInputWidget = tileGrid;
}

void GameProcessUI::Initialize() { }

void GameProcessUI::Receive(const std::string &message) {
    std::string html = message;
    html = std::regex_replace(html, std::regex("<"), "#BOLD_BEGIN");
    html = std::regex_replace(html, std::regex(">"), ":#BOLD_END ");
    html = std::regex_replace(html, std::regex("#BOLD_BEGIN"), "<b>");
    html = std::regex_replace(html, std::regex("#BOLD_END"), "</b>");
    chat->AddToContent(html + "<br>");
}


void GameProcessUI::Resize(const int width, const int height) {
	TileGrid *tileGrid = reinterpret_cast<GameProcessUI *>(CC::Get()->GetUI()->GetCurrentUIModule())->GetTileGrid();

	auto tileGridNumOfTiles = tileGrid->GetFOV() * 2 + 1;
	auto tileSize = height / tileGridNumOfTiles;
	auto tileGridSize = tileSize * tileGridNumOfTiles;
	auto tileGridHeightPadding = (height - tileGridSize) / 2;

	tileGrid->SetPosition({0, tileGridHeightPadding});
	tileGrid->SetSize({tileGridSize, tileGridSize});

	infoLabel->CountPosition(width, height);

    chat->SetSize({ width - tileGridSize, height / 2 });
    chat->SetPosition({ tileGridSize, height - chat->GetSize().y});

	functionWindow->SetPosition(static_cast<float>(tileGridSize), 0);
	functionWindow->SetSize({width - functionWindow->GetAbsolutePosition().x, chat->GetPosition().y});
}

void GameProcessUI::Draw(sf::RenderWindow *renderWindow) {
    for (auto &widget : widgets) widget->Draw(*renderWindow);
    infoLabel->Draw(renderWindow);
}

void GameProcessUI::Update(sf::Time timeElapsed) {
    UIModule::Update(timeElapsed);

    Object *objectUnderCursor = tileGrid->GetObjectUnderCursor();
    if (!objectUnderCursor) infoLabel->SetText("");
    else infoLabel->SetText(objectUnderCursor->GetName());
    
    if (showInput) { // TODO: make server-side verb
        if (!ImGui::Begin("Say:")) {
            ImGui::End();
            return;
        }

        if (inputReclaimFocus) {
            ImGui::SetKeyboardFocusHere();
            inputReclaimFocus = false;
        }
        if (ImGui::InputText("SayInput", &inputText, ImGuiInputTextFlags_EnterReturnsTrue)) {
            auto* p = new client::SendChatMessageCommand();
            std::swap(p->message, inputText);
            Connection::commandQueue.Push(p);
            showInput = false;
        }
            
        ImGui::End();
    }
}

bool GameProcessUI::HandleEvent(sf::Event event) {
    if (UIModule::HandleEvent(event))
        return true;

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        LOGD << "Open Input";
        showInput = true;
        inputReclaimFocus = true;
        return true;
    }

    return false;
}

void GameProcessUI::OpenContextMenu() {
	tileContextMenu->Open();
}

void GameProcessUI::UpdateContextMenu(network::protocol::ContextMenuData &&data) {
	tileContextMenu->SetContent(std::move(data));
}

void GameProcessUI::OpenSpawnWindow() {
	for (auto &widget : widgets)
		if (dynamic_cast<SpawnWindow *>(widget.get()))
			return;
	widgets.push_back(std::make_unique<SpawnWindow>());
}

void GameProcessUI::UpdateSpawnWindow(std::vector<network::protocol::ObjectType> &&types) {
	for (auto &widget : widgets)
		if (auto *spawnWidget = dynamic_cast<SpawnWindow *>(widget.get()))
			spawnWidget->UpdateTypes(std::forward<std::vector<network::protocol::ObjectType>>(types));
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

void InfoLabel::CountPosition(int /*width*/, int height) {
    rectangle.setPosition(0, height - rectangle.getSize().y);
    text.setPosition(5, height - rectangle.getSize().y - 2);
}

void InfoLabel::SetText(const string &s) {
    text.setString(s);
}