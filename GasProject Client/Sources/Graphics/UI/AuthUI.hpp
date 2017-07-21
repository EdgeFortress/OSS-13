#pragma once

#include "UI.hpp"
#include "State.hpp"

#include "Widget/Container.hpp"
#include "Widget/Entry.hpp"

class AuthUI : public UIModule {
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

    //sfg::Window::Ptr logWindow, regWindow;
    //sfg::Entry::Ptr login_entry, passw_entry, new_login_entry, new_passw_entry;
    Container *logWindow, *regWindow;
    Entry *my_login_entry, *my_passw_entry, *my_new_login_entry, *my_new_passw_entry;

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

    std::list<uptr<Widget>> widgets;

public:
    AuthUI(UI *ui);
    AuthUI(const AuthUI &) = delete;
    AuthUI &operator=(const AuthUI &) = delete;
    virtual ~AuthUI() = default;

    virtual void Resize(int width, int height) final;
    virtual void Draw(sf::RenderWindow *renderWindow) final { for (auto &widget : widgets) widget->Draw(*renderWindow); };
    virtual void Update(sf::Time timeElapsed) final { for (auto &widget : widgets) widget->Update(timeElapsed); }
    virtual void HandleEvent(sf::Event event) final {
        if (event.type == sf::Event::MouseButtonPressed) {
            my_login_entry->LoseFocus();
            my_passw_entry->LoseFocus();
            my_new_login_entry->LoseFocus();
            my_new_passw_entry->LoseFocus();
        }
        for (auto &widget : widgets) widget->HandleEvent(event);
    }

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