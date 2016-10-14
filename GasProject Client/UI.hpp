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

	void generateLoginWindow();
	void generateRegistrationWindow();
	void openLogin();
	void openReg();
	void login();
	void registration();

public:
	AuthUI(UI *ui);
	AuthUI(const AuthUI &) = delete;
	AuthUI &operator=(const AuthUI &) = delete;
	virtual ~AuthUI() = default;
};

class UI {
private:
	sf::RenderWindow *rendWindow;

	sfg::SFGUI m_sfgui;
	sfg::Desktop desktop;

	AuthUI authUI;

public:
	UI(sf::RenderWindow *window);
	UI(const UI &) = delete;
	UI &operator=(const UI &) = delete;
	virtual ~UI() = default;

	void HandleEvent(sf::Event event);

	sf::RenderWindow *GetRenderWindow() const { return rendWindow; }
	sfg::Desktop *GetDesktop() { return &desktop; }

	friend void MenuLoginState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
	friend void MenuServerListState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
	friend void GameLobbyState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
	friend void GameProcessState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
};