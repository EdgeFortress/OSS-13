#include "Client.hpp"
#include "Window.hpp"
#include "TileGrid.hpp"
#include "UI.hpp"
#include "State.hpp"

Block::Block(TileGrid *tileGrid) : tileGrid(tileGrid),
								   tiles(tileGrid->BlockSize,
								   vector<Tile *>(tileGrid->BlockSize)) {
	for (auto &vect : tiles)
		for (auto &tile : vect)
			tile = new Tile(this);
}

void Window::Update(sf::Time timeElapsed) {
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window->close();
	}

	window->clear(sf::Color::Black);
	controller->GetState()->DrawTileGrid();
	controller->GetState()->DrawUI();
	window->display();
}

void MenuLoginState::DrawTileGrid() const { }
void MenuServerListState::DrawTileGrid() const { }
void GameLobbyState::DrawTileGrid() const { }
void GameProcessState::DrawTileGrid() const { }

void MenuLoginState::DrawUI() const { }
void MenuServerListState::DrawUI() const { }
void GameLobbyState::DrawUI() const { }
void GameProcessState::DrawUI() const { }