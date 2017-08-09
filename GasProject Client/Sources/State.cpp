#include "State.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/TileGrid/TileGrid.hpp"
#include "Graphics/UI/UI.hpp"
#include "Graphics/UI/UIModule/AuthUI.hpp"
#include "Graphics/UI/UIModule/GameListUI.hpp"
#include "Graphics/UI/UIModule/GameProcessUI.hpp"

void MenuLoginState::Initialize() const { 
    CC::Get()->GetWindow()->GetUI()->GetAuthUI()->Show();
}

void MenuGameListState::Initialize() const { 
    CC::Get()->GetWindow()->GetUI()->GetGameListUI()->Show();
}

void GameLobbyState::Initialize() const { }
void GameProcessState::Initialize() const { 
    CC::Get()->GetWindow()->GetUI()->GetGameProcessUI()->Show();
}

void MenuLoginState::Ending() const {  
    CC::Get()->GetWindow()->GetUI()->GetAuthUI()->Hide();
}

void MenuGameListState::Ending() const { 
    CC::Get()->GetWindow()->GetUI()->GetGameListUI()->Hide();
}

void GameLobbyState::Ending() const { }
void GameProcessState::Ending() const { }

void MenuLoginState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void MenuGameListState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void GameLobbyState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const { }
void GameProcessState::DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const {
	tileGrid->Draw(render_window);
}

void MenuLoginState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
	Window *window = CC::Get()->GetWindow();
	AuthUI *authUI = window->GetUI()->GetAuthUI();

	if (authUI->comState != AuthUI::ComState::NOTHING) {
		AuthUI::ServerAnswer answer = window->GetUI()->GetAuthUI()->GetAnswer();
		if (answer.isAnswer) {
			if (authUI->comState == AuthUI::ComState::LOGIN) {
				if (answer.result) {
					CC::log << "You logged in succesfully" << endl;
					CC::Get()->SetState(new MenuGameListState);
				}
				else {
					CC::log << "You did not log in" << endl;
				}
			}
			if (authUI->comState == AuthUI::ComState::REGISTRATION) {
				if (answer.result)
					CC::log << "You are succesfully registered" << endl;
				else
					CC::log << "Problems with registration" << endl;
				authUI->openLogin();
			}
			authUI->comState = AuthUI::ComState::NOTHING;
		}
	}

	window->GetUI()->Lock();
	//window->GetUI()->Update(timeElapsed);
	window->GetUI()->DrawMenuBackground(render_window);
	authUI->Update(timeElapsed);
	authUI->Draw(render_window);
	//window->GetUI()->Draw(render_window);
	window->GetUI()->Unlock();
}
void MenuGameListState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
	Window *window = CC::Get()->GetWindow();
	window->GetUI()->Lock();
	//window->GetUI()->Update(timeElapsed);
	window->GetUI()->DrawMenuBackground(render_window);
	//window->GetUI()->Draw(render_window);
	window->GetUI()->GetGameListUI()->Draw(render_window);
	window->GetUI()->Unlock();
}
void GameLobbyState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const { }
void GameProcessState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
	Window *window = CC::Get()->GetWindow();
	GameProcessUI* gameProcessUI = window->GetUI()->GetGameProcessUI();
	window->GetUI()->Lock();
	//window->GetUI()->Update(timeElapsed);
	gameProcessUI->Draw(render_window, timeElapsed);
	gameProcessUI->Draw(render_window);
	//window->GetUI()->Draw(render_window);
	window->GetUI()->Unlock();
}

void MenuLoginState::HandleEvent(sf::Event event) const {
	////////if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
	////////	CC::Get()->GetWindow()->GetUI()->GetAuthUI()->changeFocus();
	////////if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
	////////	CC::Get()->GetWindow()->GetUI()->GetAuthUI()->accountDataEnter();

	//if (event.type == sf::Event::TextEntered)
	//	CC::Get()->GetWindow()->GetUI()->GetAuthUI()->HandleEvent(event);
	//if (event.type == sf::Event::KeyPressed)
	//	if (event.key.code == sf::Keyboard::BackSpace || sf::Keyboard::Left || sf::Keyboard::Right)
	//		CC::Get()->GetWindow()->GetUI()->GetAuthUI()->HandleEvent(event);
	//if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	//	CC::Get()->GetWindow()->GetUI()->GetAuthUI()->HandleEvent(event);
	//if (event.type == sf::Event::MouseMoved)
	//	CC::Get()->GetWindow()->GetUI()->GetAuthUI()->HandleEvent(event);
}
void MenuGameListState::HandleEvent(sf::Event event) const {
	CC::Get()->GetWindow()->GetUI()->GetGameListUI()->HandleEvent(event);
}
void GameLobbyState::HandleEvent(sf::Event event) const { }
void GameProcessState::HandleEvent(sf::Event event) const {
	CC::Get()->GetWindow()->GetUI()->GetGameProcessUI()->HandleEvent(event);
	Chat *chat = CC::Get()->GetWindow()->GetUI()->GetGameProcessUI()->GetChat();

	static bool playing = true;
	bool chatting = chat->GetState();

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
		playing = !playing, chatting = !chatting, chat->SetState(chatting);
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x > chat->GetXPos() && event.mouseButton.y > chat->GetYPos())
		playing = false, chatting = true, chat->SetState(chatting);
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x < chat->GetXPos())
		playing = true, chatting = false, chat->SetState(chatting);

	if (playing) CC::Get()->GetWindow()->GetTileGrid()->HandleEvent(event);

	if (event.type == sf::Event::MouseMoved) {
		Object *obj = CC::Get()->GetWindow()->GetTileGrid()->GetObjectByPixel(event.mouseMove.x, event.mouseMove.y);
		if (obj != nullptr) CC::Get()->GetWindow()->GetUI()->GetGameProcessUI()->GetInfoLabel()->SetText(obj->GetName());
		else CC::Get()->GetWindow()->GetUI()->GetGameProcessUI()->GetInfoLabel()->SetText("");
	}

	if (chatting) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Return)
				chat->Send();
			if (event.key.code == sf::Keyboard::BackSpace)
				chat->DeleteSymbol();
			if (event.key.code == sf::Keyboard::Left)
				chat->MoveLeft();
			if (event.key.code == sf::Keyboard::Right)
				chat->MoveRight();
			if (event.key.code == sf::Keyboard::Up)
				chat->ScrollUp();
			if (event.key.code == sf::Keyboard::Down)
				chat->ScrollDown();
		}
		if (event.type == sf::Event::TextEntered) {
			wchar_t c = wchar_t(event.text.unicode);
			if (c != '\r' && c != '\t' && c != '\b')
				chat->SetSymbol(c);
		}
	}
}

void MenuLoginState::Update(sf::Time timeElapsed) const { }
void MenuGameListState::Update(sf::Time timeElapsed) const {
	CC::Get()->GetWindow()->GetUI()->GetGameListUI()->Update(timeElapsed);
}
void GameLobbyState::Update(sf::Time timeElapsed) const { }
void GameProcessState::Update(sf::Time timeElapsed) const {
	Window *window = CC::Get()->GetWindow();
	window->GetUI()->GetGameProcessUI()->Update(timeElapsed);
	window->GetTileGrid()->Update(timeElapsed);
}