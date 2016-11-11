#include "Client.hpp"
#include "Window.hpp"
#include "TileGrid/TileGrid.hpp"
#include "UI/UI.hpp"
#include "State.hpp"

void Window::loadTextures(list<uptr<Texture>> &textures, list<uptr<Sprite>> &sprites) {
    Texture *t = new Texture("Images/Human.png", 32, 1);
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Mob, 0, 0, true, 1)));
    textures.push_back(uptr<Texture>(t));
    t = new Texture("Images/Turfs.png", 32, 2);
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Wall, 0, 0, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Floor, 1, 1, false, 1)));
    textures.push_back(uptr<Texture>(t));
}

void Window::Update(sf::Time timeElapsed) {
    sf::Event event;
    State *state = CC::Get()->GetState();
    while (window->pollEvent(event)) {
        ui->HandleEvent(event);
        if (state)
            state->HandleEvent(event);
        if (event.type == sf::Event::Resized)
            Resize(event.size.width, event.size.height);
        if (event.type == sf::Event::Closed)
            window->close();
    }

    window->resetGLStates();
    window->clear(sf::Color::Black);
    if (state) {
        state->DrawTileGrid(window.get(), tileGrid.get());
        state->DrawUI(window.get(), timeElapsed);
    }
    window->display();
    sf::sleep(sf::milliseconds(10));
}

void Window::Resize(const int newWidth, const int newHeight) {
    width = newWidth; height = newHeight;
    sf::FloatRect visibleArea(0, 0, float(width), float(height));
    window->setView(sf::View(visibleArea));
    tileGrid->Resize(width, height);
    for (auto &sprite : sprites)
        sprite->Resize(tileGrid->GetTileSize());
}

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
                } else {
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
    window->GetUI()->Update(timeElapsed);
    window->GetUI()->Draw(render_window);
    window->GetUI()->Unlock();
}

void MenuGameListState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
    Window *window = CC::Get()->GetWindow();
    window->GetUI()->Lock();
    window->GetUI()->Update(timeElapsed);
    window->GetUI()->Draw(render_window);
    window->GetUI()->Unlock();
}
void GameLobbyState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const { }
void GameProcessState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const { }

void MenuLoginState::HandleEvent(sf::Event event) const {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
        CC::Get()->GetWindow()->GetUI()->GetAuthUI()->ChangeFocus();
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
        CC::Get()->GetWindow()->GetUI()->GetAuthUI()->AccountDataEnter();
}

void MenuGameListState::HandleEvent(sf::Event event) const { }
void GameLobbyState::HandleEvent(sf::Event event) const { }
void GameProcessState::HandleEvent(sf::Event event) const { }