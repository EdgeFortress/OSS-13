#pragma once

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Common/Useful.hpp"

class GameListUI;
class AuthUI;
class GameProcessUI;

class UI;

class UIModule {
protected:
    UI *ui;

public:
    UIModule(UI *ui);
    UIModule(const UIModule &) = delete;
    UIModule &operator=(const UIModule &) = delete;
    virtual ~UIModule() = default;

    virtual void Resize(int width, int height) = 0;
    virtual void Draw(sf::RenderWindow *renderWindow) = 0;
    virtual void Update(sf::Time timeElapsed) = 0;

    virtual void Hide() = 0;
    virtual void Show() = 0;
};


class UI {
private:
    sfg::SFGUI m_sfgui;
    sfg::Desktop desktop;

    sf::Font font;

    sf::Texture background;
    sf::Sprite background_sprite;

    uptr<AuthUI> authUI;
    uptr<GameListUI> gamelistUI;
    uptr<GameProcessUI> gameProcessUI;

    std::mutex UImutex;

public:
    UI();
    UI(const UI &) = delete;
    UI &operator=(const UI &) = delete;
    virtual ~UI() = default;

    void Resize(int width, int height);
    void HandleEvent(sf::Event event);
    void Update(sf::Time timeElapsed);
    void Draw(sf::RenderWindow *render_window);

    void DrawMenuBackground(sf::RenderWindow *render_window);

    void Lock();
    void Unlock();

    const sf::Font &GetFont() const { return font; }
    sfg::Desktop *GetDesktop() { return &desktop; }

    AuthUI *GetAuthUI() { return authUI.get(); }
    GameListUI *GetGameListUI() { return gamelistUI.get(); }
    GameProcessUI *GetGameProcessUI() { return gameProcessUI.get(); }
};