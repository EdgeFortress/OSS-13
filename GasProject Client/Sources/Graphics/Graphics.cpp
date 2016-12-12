#include "Client.hpp"
#include "Window.hpp"
#include "Network.hpp"
#include "TileGrid/TileGrid.hpp"

#include "Graphics/UI/UI.hpp"
#include "Graphics/UI/AuthUI.hpp"
#include "Graphics/UI/GameProcessUI.hpp"

#include "State.hpp"

void Window::Initialize() {
    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
    width = static_cast<int>(0.8 * videoMode.width);
    height = static_cast<int>(0.8 * videoMode.height);
    window.reset(new RenderWindow(sf::VideoMode(width, height), "GasProjectClient"));
    resize(window->getSize().x, window->getSize().y);
}

void Window::Update(sf::Time timeElapsed) {
    sf::Event event;

    window->setKeyRepeatEnabled(true);

    State *state = CC::Get()->GetState();
    while (window->pollEvent(event)) {
        ui->HandleEvent(event);
        if (state) {          
            state->HandleEvent(event);
        }
        if (event.type == sf::Event::Resized)
            resize(event.size.width, event.size.height);
        if (event.type == sf::Event::Closed)
            window->close();
    }

    animationTime += timeElapsed;
    if (animationTime >= sf::seconds(1)) {
        animationTime = sf::Time::Zero;
        for (auto &sprite : sprites)
            if (sprite->Animated())
                sprite->UpdateFrame();
    }

    window->resetGLStates();
    window->clear(sf::Color::Black);
    if (state) {
        state->DrawTileGrid(window.get(), tileGrid.get());
        state->Update(timeElapsed);
        state->DrawUI(window.get(), timeElapsed);
    }
    window->display();
    //fps_exceed();
    sf::sleep(sf::milliseconds(10));
}

void Window::resize(const int newWidth, const int newHeight) {
    width = newWidth; height = newHeight;
    sf::FloatRect visibleArea(0, 0, float(width), float(height));
    window->setView(sf::View(visibleArea));
    tileGrid->Resize(width, height);
    ui->Resize(width, height);
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
    window->GetUI()->DrawMenuBackground(render_window);
    window->GetUI()->Draw(render_window);
    window->GetUI()->Unlock();
}
void MenuGameListState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
    Window *window = CC::Get()->GetWindow();
    window->GetUI()->Lock();
    window->GetUI()->Update(timeElapsed);
    window->GetUI()->DrawMenuBackground(render_window);
    window->GetUI()->Draw(render_window);
    window->GetUI()->Unlock();
}
void GameLobbyState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const { }
void GameProcessState::DrawUI(sf::RenderWindow *render_window, sf::Time timeElapsed) const {
    Window *window = CC::Get()->GetWindow();
    GameProcessUI* gameProcessUI = window->GetUI()->GetGameProcessUI();
    window->GetUI()->Lock();
    window->GetUI()->Update(timeElapsed);
    gameProcessUI->Draw(render_window, timeElapsed);
    window->GetUI()->Draw(render_window);
    window->GetUI()->Unlock();
}

void MenuLoginState::HandleEvent(sf::Event event) const {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
        CC::Get()->GetWindow()->GetUI()->GetAuthUI()->ChangeFocus();
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
        CC::Get()->GetWindow()->GetUI()->GetAuthUI()->AccountDataEnter();
}
void MenuGameListState::HandleEvent(sf::Event event) const { }
void GameLobbyState::HandleEvent(sf::Event event) const { }
void GameProcessState::HandleEvent(sf::Event event) const {
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
        if (obj != nullptr) CC::Get()->GetWindow()->GetUI()->GetGameProcessUI()->GetInfoLabel()->SetText(obj->name);
        else CC::Get()->GetWindow()->GetUI()->GetGameProcessUI()->GetInfoLabel()->SetText("");
    }
 
    if (chatting) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Return)
                chat->Send();
            if (event.key.code == sf::Keyboard::BackSpace)
                chat->DeleteSymbol();
        }
        if (event.type == sf::Event::TextEntered) {
            wchar_t c = wchar_t(event.text.unicode);
            if (c != '\r' && c != '\t' && c != '\b')
                chat->SetSymbol(c);
        }
    }
}

void MenuLoginState::Update(sf::Time timeElapsed) const { }
void MenuGameListState::Update(sf::Time timeElapsed) const { }
void GameLobbyState::Update(sf::Time timeElapsed) const { }
void GameProcessState::Update(sf::Time timeElapsed) const {
    Window *window = CC::Get()->GetWindow();
    window->GetUI()->GetGameProcessUI()->Update();
    window->GetTileGrid()->Update(timeElapsed);
}

void Window::loadTextures(list<uptr<Texture>> &textures, list<uptr<Sprite>> &sprites) {
    Texture *t = new Texture("Images/Human.png", 32, 1);
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Mob, 0, 0, true, 1)));
    textures.push_back(uptr<Texture>(t));
    t = new Texture("Images/Turfs.png", 32, 2);
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Wall, 0, 0, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Floor, 1, 1, false, 1)));
    textures.push_back(uptr<Texture>(t));
    t = new Texture("Images/Space.png", 32, 26);
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Space, 0, 0, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 1), 1, 1, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 2), 2, 2, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 3), 3, 3, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 4), 4, 4, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 5), 5, 5, false, 5)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 6), 6, 10, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 7), 7, 14, false, 5)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 8), 8, 19, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 9), 9, 23, false, 5)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 10), 10, 28, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 11), 11, 32, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 12), 12, 36, false, 5)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 13), 13, 41, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 14), 14, 45, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 15), 15, 49, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 16), 16, 53, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 17), 17, 57, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 18), 18, 61, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 19), 19, 65, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 20), 20, 69, false, 4)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 21), 21, 73, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 22), 22, 74, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 23), 23, 75, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 24), 24, 76, false, 1)));
    sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 25), 25, 77, false, 1)));
    textures.push_back(uptr<Texture>(t));
}