#pragma once

#include "State.hpp"
#include "network.hpp"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
class UI;

class AuthUI {

	struct ServerAnswer{
		bool isAnswer;
		bool result;

		explicit ServerAnswer(bool result) : isAnswer(true), result(result) {}
		ServerAnswer() : isAnswer(false) {}
		ServerAnswer(const ServerAnswer &serverAnswer) = default;
		ServerAnswer &operator=(const ServerAnswer &serverAnswer) = default;
	};

private:
	UI *ui;

	sfg::Window::Ptr logWindow, regWindow;
	sfg::Entry::Ptr login_entry, passw_entry, new_login_entry, new_passw_entry;

	ServerAnswer serverAnswer;
	std::mutex mutex;

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
		//logg << "SetAnswer:" << result << endl;
		serverAnswer = ServerAnswer(result);
		mutex.unlock();
    }

	ServerAnswer GetAnswer() {
		mutex.lock();
		//logg << "GetAnswer:";
		ServerAnswer temp = serverAnswer;
		//logg << temp.isAnswer << temp.result << endl;
		serverAnswer.isAnswer = false;
		mutex.unlock();
		return temp;
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