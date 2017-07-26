#include <SFML/Graphics.hpp>

#include "Client.hpp"
#include "AuthUI.hpp"
#include "Network.hpp"

#include "Widget/Label.hpp"
#include "Widget/Button.hpp"
#include "Widget//Entry.hpp"

void AuthUI::AccountDataEnter() {
    if (my_login_entry->HasFocus() || my_passw_entry->HasFocus())
        this->login();
    if (my_new_login_entry->HasFocus() || my_new_passw_entry->HasFocus())
        this->registration();
}


AuthUI::AuthUI(UI *ui) : UIModule(ui) {
    comState = ComState::NOTHING;
    generateLoginWindow();
    generateRegistrationWindow();
    Hide();
}

void OnButtonClick() {
    CC::log << "ButtonPressed" << std::endl;
}

void AuthUI::generateLoginWindow() {
    my_login_entry = new Entry(sf::Vector2f(400, 25), sf::Color(193, 205, 205), ui->GetFont());
    my_passw_entry = new Entry(sf::Vector2f(400, 25), sf::Color(193, 205, 205), ui->GetFont(), true);
    logWindow = new Container(sf::Vector2f(600, 103));
    logWindow->AddItem(new Label(L"Login", ui->GetFont(), 16), sf::Vector2f(0, 0));
    logWindow->AddItem(new Label(L"Password", ui->GetFont(), 16), sf::Vector2f(0, 26));
    logWindow->AddItem(my_login_entry, sf::Vector2f(100, 0));
    logWindow->AddItem(my_passw_entry, sf::Vector2f(100, 26));
    logWindow->AddItem(new Button(L"Enter", sf::Vector2f(100, 51), sf::Color::White, ui->GetFont(), std::bind(&AuthUI::login, this)), sf::Vector2f(500, 0));
    logWindow->AddItem(new Button(L"Registration", sf::Vector2f(100, 51), sf::Color::White, ui->GetFont(), std::bind(&AuthUI::openReg, this)), sf::Vector2f(500, 52));
    logWindow->SetPosition(200, 200);
    logWindow->SetBackground(sf::Color::Transparent);
    widgets.push_back(uptr<Container>(logWindow));

    my_login_entry->GrabFocus();
}

void AuthUI::generateRegistrationWindow() {
    my_new_login_entry = new Entry(sf::Vector2f(400, 25), sf::Color(193, 205, 205), ui->GetFont());
    my_new_passw_entry = new Entry(sf::Vector2f(400, 25), sf::Color(193, 205, 205), ui->GetFont(), true);
    regWindow = new Container(sf::Vector2f(650, 72));
    regWindow->AddItem(new Label(L"Login", ui->GetFont(), 16), sf::Vector2f(0, 21));
    regWindow->AddItem(new Label(L"Password", ui->GetFont(), 16), sf::Vector2f(0, 47));
    regWindow->AddItem(my_new_login_entry, sf::Vector2f(100, 21));
    regWindow->AddItem(my_new_passw_entry, sf::Vector2f(100, 47));
    regWindow->AddItem(new Button(L"Create account", sf::Vector2f(150, 51), sf::Color::White, ui->GetFont(), std::bind(&AuthUI::registration, this)), sf::Vector2f(500, 21));
    regWindow->AddItem(new Button(L"Close", sf::Vector2f(100, 20), sf::Color::White, ui->GetFont(), std::bind(&AuthUI::closeReg, this)), sf::Vector2f(500, 0));
    regWindow->SetPosition(400, 400);
    regWindow->SetBackground(sf::Color::Transparent);
    widgets.push_back(uptr<Container>(regWindow));
}

void AuthUI::openLogin() {
    regWindow->Hide();
    my_new_login_entry->Clear();
    my_new_passw_entry->Clear();
    my_login_entry->GrabFocus();
    logWindow->Show();
}

void AuthUI::openReg() {
    my_login_entry->Clear();
    my_passw_entry->Clear();
    my_login_entry->LoseFocus();
    my_passw_entry->LoseFocus();
    my_new_login_entry->GrabFocus();
    regWindow->Show();
}

void AuthUI::closeReg() {
    regWindow->Hide();
}

void AuthUI::login() {
    if (comState == ComState::NOTHING) {
        Connection::commandQueue.Push(new AuthorizationClientCommand(my_login_entry->GetText(), my_passw_entry->GetText()));
        comState = ComState::LOGIN;
    }
    else
        CC::log << "Wait for answer to previous command" << std::endl;
}

void AuthUI::registration() {
    if (comState == ComState::NOTHING) {
        Connection::commandQueue.Push(new RegistrationClientCommand(my_new_login_entry->GetText(), my_new_passw_entry->GetText()));
        comState = ComState::REGISTRATION;
    }
    else
        CC::log << "Wait for answer to previous command" << std::endl;
}

void AuthUI::Resize(int width, int height) {
    logWindow->SetPosition(sf::Vector2f((width - logWindow->GetPosition().x) / 10,
                                        (height - logWindow->GetPosition().y) / 3));
    //regWindow->SetPosition(sf::Vector2f((width - regWindow->GetPosition().x) / 10,
    //                                    (height - regWindow->GetPosition().y) / 3));
}

void AuthUI::Show() {
    openLogin();
}

void AuthUI::Hide() {
    regWindow->Hide();
    logWindow->Hide();
}

void AuthUI::ChangeFocus() {
    if (my_login_entry->HasFocus()) {
        my_login_entry->LoseFocus();
        my_passw_entry->GrabFocus();
    }
    else if (my_passw_entry->HasFocus()) {
        my_passw_entry->LoseFocus();
        my_login_entry->GrabFocus();
    }
    if (my_new_login_entry->HasFocus()) {
        my_new_login_entry->LoseFocus();
        my_new_passw_entry->GrabFocus();
    }
    else if (my_new_passw_entry->HasFocus()) {
        my_new_passw_entry->LoseFocus();
        my_new_login_entry->GrabFocus();
    }
}