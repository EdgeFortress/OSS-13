#pragma once

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/System.hpp>

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

    virtual void Draw() = 0;

    virtual void Hide() = 0;
    virtual void Show() = 0;
};


class UI {
private:
    sf::RenderWindow *rendWindow;

    sfg::SFGUI m_sfgui;
    sfg::Desktop desktop;

    uptr<AuthUI> authUI;
    uptr<GameListUI> gamelistUI;
    uptr<GameProcessUI> gameProcessUI;

    std::mutex UImutex;

public:
    UI(sf::RenderWindow *window);
    UI(const UI &) = delete;
    UI &operator=(const UI &) = delete;
    virtual ~UI() = default;

    void HandleEvent(sf::Event event);
    void Update(sf::Time timeElapsed);
    void Draw(sf::RenderWindow *render_window);

    void Lock();
    void Unlock();

    sf::RenderWindow *GetRenderWindow() const { return rendWindow; }
    sfg::Desktop *GetDesktop() { return &desktop; }
    AuthUI *GetAuthUI() { return authUI.get(); }
    GameListUI *GetGameListUI() { return gamelistUI.get(); }
    GameProcessUI *GetGameProcessUI() { return gameProcessUI.get(); }
};