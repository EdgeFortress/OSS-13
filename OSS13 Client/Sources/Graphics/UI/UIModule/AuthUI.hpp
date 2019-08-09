#pragma once

#include <mutex>

#include "Graphics/UI/UIModule/UIModule.hpp"
#include "Graphics/UI/Widget/Container.hpp"
#include "Graphics/UI/Widget/Entry.hpp"

class UI;

class AuthUI : public UIModule {
public:
    AuthUI(UI *ui);
    AuthUI(const AuthUI &) = delete;
    AuthUI &operator=(const AuthUI &) = delete;
    virtual ~AuthUI() = default;

    virtual void Initialize() final;

    virtual void Resize(int width, int height) final;
    virtual void Draw(sf::RenderWindow *renderWindow) final;
    virtual void Update(sf::Time timeElapsed) final;

    void SetServerAnswer(bool result);

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
    };

    ServerAnswer serverAnswer;
    ComState comState;

    ServerAnswer getAnswer();

	// Generate functions
    void generateLoginWindow();
    void generateRegistrationWindow();

	// Event Handlers
    void openReg();
    void closeReg();
    void login();
    void registration();
};