#pragma once

#include "State.hpp"
#include "network.hpp"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
class UI;

class AuthUI {
private:
	UI *ui;

	sfg::Window::Ptr logWindow, regWindow;
	sfg::Entry::Ptr login_entry, passw_entry, new_login_entry, new_passw_entry;

    bool serverAnswer;
    bool result;

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
        serverAnswer = true;
        this->result = result;
    }

    friend void MenuLoginWaitingState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
};

class UI {
private:
    ClientController *clientController;
	sf::RenderWindow *rendWindow;

	sfg::SFGUI m_sfgui;
	sfg::Desktop desktop;

	AuthUI authUI;

public:
	UI(ClientController *clientController, sf::RenderWindow *window);
	UI(const UI &) = delete;
	UI &operator=(const UI &) = delete;
	virtual ~UI() = default;

	void HandleEvent(sf::Event event);

    ClientController *GetClientController() const { return clientController; }
	sf::RenderWindow *GetRenderWindow() const { return rendWindow; }
	sfg::Desktop *GetDesktop() { return &desktop; }
    AuthUI *GetAuthUI() { return &authUI; }

	friend void MenuLoginState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
    friend void MenuLoginWaitingState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
	friend void MenuServerListState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
	friend void GameLobbyState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
	friend void GameProcessState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
};