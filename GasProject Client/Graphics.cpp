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
	window->resetGLStates();
	while (window->pollEvent(event)) {
		ui->HandleEvent(event);
		if (event.type == sf::Event::Closed)
			window->close();
	}

	window->clear(sf::Color::Black);
	controller->GetState()->DrawTileGrid();
	controller->GetState()->DrawUI(window.get(), timeElapsed);
	window->display();
}

void MenuLoginState::DrawTileGrid() const { }
void MenuServerListState::DrawTileGrid() const { }
void GameLobbyState::DrawTileGrid() const { }
void GameProcessState::DrawTileGrid() const { }

void MenuLoginState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const {
	clientController->GetWindow()->ui->desktop->Update(timeElapsed.asSeconds());
	clientController->GetWindow()->ui->m_sfgui.Display(*window);
}
void MenuServerListState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const { }
void GameLobbyState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const { }
void GameProcessState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const { }