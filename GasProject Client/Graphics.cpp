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

void Window::loadTextures() {
	new Texture(1, "Images/Human.png", 32, textures, 1);
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
	controller->GetState()->DrawTileGrid(window.get(), tileGrid.get());
	controller->GetState()->DrawUI(window.get(), timeElapsed);
	window->display();
}

void MenuLoginState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void MenuLoginWaitingState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void MenuServerListState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void GameLobbyState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void GameProcessState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const {
	tileGrid->Draw(render_window);
}

void MenuLoginState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
	Window *window = clientController->GetWindow();
	window->GetUI()->desktop.Update(timeElapsed.asSeconds());
	window->GetUI()->m_sfgui.Display(*render_window);
}

void MenuLoginWaitingState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
    Window *window = clientController->GetWindow();

    bool endWaiting = true;
	AuthUI::ServerAnswer answer = window->GetUI()->GetAuthUI()->GetAnswer();
    if (loginWaiting) {
        if (answer.isAnswer) {
			if (answer.result) {
				cout << "You logged in succesfully" << endl;
				clientController->SetState(new MenuServerListState(clientController));
			}
			else
				cout << "Wrong login data" << endl;
        } else {
            endWaiting = false;
        }
    }
    if (regWaiting) {
        if (answer.isAnswer) {
			if (answer.result)
				cout << "You are succesfully registered" << endl;
			else
				cout << "Problems with registration" << endl;
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

void Tile::SetSprite(int textureIndex, int num, int frame) {
	for (const uptr<Texture> &texture : clientController->GetWindow()->GetTextures())
		if (texture->GetKey() == textureIndex) {
			sprite = new Sprite();
			sprite->SetTexture(texture.get());
			sprite->SetSpriteState(num, -1, frame);
			return;
		}
}

void Object::SetSprite(int textureIndex, int num, int direction, int frame)
{
	for (const uptr<Texture> &texture : clientController->GetWindow()->GetTextures())
		if (texture->GetKey() == textureIndex) {
			sprite->SetTexture(texture.get());
			sprite->SetSpriteState(num, direction, frame);
			return;
		}
}