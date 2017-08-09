#pragma once

#include "State.hpp"

#include "Graphics/UI/UIModule/UIModule.hpp"
#include "Graphics/UI/Widget/Container.hpp"
#include "Graphics/UI/Widget/Entry.hpp"

class UI;

class AuthUI : public UIModule {
private:
	Container *logWindow, *regWindow;
	Entry *my_login_entry, *my_passw_entry, *my_new_login_entry, *my_new_passw_entry;

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

    ServerAnswer serverAnswer;
    std::mutex mutex;
    ComState comState;

	// Generate functions
    void generateLoginWindow();
    void generateRegistrationWindow();

	// Event Handlers
    void openLogin();
    void openReg();
    void closeReg();
    void login();
    void registration();
	//void changeFocus();
	//void accountDataEnter();

    std::list<uptr<Widget>> widgets;

    Widget *curInputWidget;

public:
    AuthUI(UI *ui);
    AuthUI(const AuthUI &) = delete;
    AuthUI &operator=(const AuthUI &) = delete;
    virtual ~AuthUI() = default;

    virtual void Resize(int width, int height) final;
	virtual void Draw(sf::RenderWindow *renderWindow) final;
	virtual void Update(sf::Time timeElapsed) final;
	virtual void HandleEvent(sf::Event event) final;

	void SetServerAnswer(bool result);
	ServerAnswer GetAnswer();

    virtual void Hide() final;
    virtual void Show() final;

    friend void MenuLoginState::DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const;
};