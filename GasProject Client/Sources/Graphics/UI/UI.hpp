#pragma once

#include "State.hpp"
#include "network.hpp"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class UI;

class UIModule {
protected:
    UI *ui;

public:
    UIModule(UI *ui);
    UIModule(const UIModule &) = delete;
    UIModule &operator=(const UIModule &) = delete;
    virtual ~UIModule() = default;

    virtual void Hide() = 0;
    virtual void Show() = 0;
};

class AuthUI : UIModule {
private:
    enum class ComState : char {
        NOTHING = 0,
        LOGIN,
        REGISTRATION
    };

    struct ServerAnswer {
        bool isAnswer;
        bool result;

        explicit ServerAnswer(bool result) : isAnswer(true), result(result) {}
        ServerAnswer() : isAnswer(false) {}
        ServerAnswer(const ServerAnswer &serverAnswer) = default;
        ServerAnswer &operator=(const ServerAnswer &serverAnswer) = default;
    };

    sfg::Window::Ptr logWindow, regWindow;
    sfg::Entry::Ptr login_entry, passw_entry, new_login_entry, new_passw_entry;

    ServerAnswer serverAnswer;
    std::mutex mutex;
    ComState comState;

    void generateLoginWindow();
    void generateRegistrationWindow();
    void openLogin();
    void openReg();
    void closeReg();
    void login();
    void registration();

public:
    AuthUI(UI *ui);
    AuthUI(const AuthUI &) = delete;
    AuthUI &operator=(const AuthUI &) = delete;
    virtual ~AuthUI() = default;

    void SetServerAnswer(bool result) {
        mutex.lock();
        serverAnswer = ServerAnswer(result);
        mutex.unlock();
    }

    ServerAnswer GetAnswer() {
        mutex.lock();
        ServerAnswer temp = serverAnswer;
        serverAnswer.isAnswer = false;
        mutex.unlock();
        return temp;
    }

    virtual void Hide() final;
    virtual void Show() final;

	void ChangeFocus();
	void AccountDataEnter();

    friend void MenuLoginState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
};

struct GameRow {
    int id;
    string title;
    int num_of_players;

    std::shared_ptr<sfg::Box> box;

    GameRow(int id, string title, int num_of_players);

    void join();
};

class GameListUI : UIModule {
private:
    sfg::Window::Ptr gamelistWindow;
    sfg::Box::Ptr gamesBox;

    std::list<uptr<GameRow>> games;

    void generateGamelistWindow();

    void update();

public:
    GameListUI(UI *ui);
    GameListUI(const GameListUI &) = delete;
    GameListUI &operator=(const GameListUI &) = delete;
    virtual ~GameListUI() = default;

    void AddGame(int id, string title, int num_of_players);
    void Clear();

    virtual void Hide() final;
    virtual void Show() final;
};

class UI {
private:
	sf::RenderWindow *rendWindow;

	sfg::SFGUI m_sfgui;
	sfg::Desktop desktop;

	AuthUI authUI;
    GameListUI gamelistUI;

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
    AuthUI *GetAuthUI() { return &authUI; }
    GameListUI *GetGameListUI() { return &gamelistUI; };
};