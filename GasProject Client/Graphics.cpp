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
		ui->HandleEvent(event);
		if (event.type == sf::Event::Closed)
			window->close();
	}

	window->resetGLStates();
	window->clear(sf::Color::Black);
	controller->GetState()->DrawTileGrid();
	controller->GetState()->DrawUI(window.get(), timeElapsed);
	window->display();
}

void MenuLoginState::DrawTileGrid() const { }
void MenuLoginWaitingState::DrawTileGrid() const { }
void MenuServerListState::DrawTileGrid() const { }
void GameLobbyState::DrawTileGrid() const { }
void GameProcessState::DrawTileGrid() const { }

void MenuLoginState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
	Window *window = clientController->GetWindow();
	window->GetUI()->desktop.Update(timeElapsed.asSeconds());
	window->GetUI()->m_sfgui.Display(*render_window);
}

void MenuLoginWaitingState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
    Window *window = clientController->GetWindow();

    bool endWaiting = true;
    if (loginWaiting) {
        if (window->GetUI()->GetAuthUI()->serverAnswer) {
            bool result = window->GetUI()->GetAuthUI()->result;
			if (result)
				cout << "You logged in succesfully" << endl;
        } else {
            endWaiting = false;
        }
    }
    if (regWaiting) {
        if (window->GetUI()->GetAuthUI()->serverAnswer) {
            bool result = window->GetUI()->GetAuthUI()->result;
			if (result)
				cout << "You are succesfully registered" << endl;
            window->GetUI()->GetAuthUI()->openLogin();
        } else {
            endWaiting = false;
        }
    }

    window->GetUI()->desktop.Update(timeElapsed.asSeconds());
    window->GetUI()->m_sfgui.Display(*render_window);

    if (endWaiting) clientController->SetState(new MenuLoginState(clientController));
}

void MenuServerListState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const { }
void GameLobbyState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const { }
void GameProcessState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const { }