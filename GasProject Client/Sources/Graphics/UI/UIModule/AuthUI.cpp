#include "AuthUI.hpp"

#include <SFML/Graphics.hpp>
#include <typeinfo>

#include "Graphics/UI/UI.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Network.hpp"

#include "Graphics/UI/Widget/Label.hpp"
#include "Graphics/UI/Widget/Button.hpp"
#include "Graphics/UI/Widget/Entry.hpp"

AuthUI::AuthUI(UI *ui) : UIModule(ui) {
    comState = ComState::NOTHING;
    generateRegistrationWindow();
    generateLoginWindow();
	regWindow->Hide();
	curInputWidget = logWindow;
}

void AuthUI::Initialize() { }

void AuthUI::Resize(int width, int height) {
    logWindow->SetPosition(uf::vec2i(width / 10, height / 4));
    regWindow->SetPosition(uf::vec2i(width / 8, height / 2));
}

void AuthUI::Draw(sf::RenderWindow *renderWindow) {
    ui->DrawMenuBackground(renderWindow);
    UIModule::Draw(renderWindow);
};

void AuthUI::Update(sf::Time timeElapsed) {
    if (comState != AuthUI::ComState::NOTHING) {
        AuthUI::ServerAnswer answer = getAnswer();
        if (answer.isAnswer) {
            if (comState == AuthUI::ComState::LOGIN) {
                if (answer.result) {
                    CC::log << "You logged in succesfully" << std::endl;
                    ui->ChangeModule<GameListUI>();
                }
                else {
                    CC::log << "You did not log in" << std::endl;
                }
            }
            if (comState == AuthUI::ComState::REGISTRATION) {
                if (answer.result) {
                    CC::log << "You are succesfully registered" << std::endl;
                    closeReg();
                }
                else
                    CC::log << "Problems with registration" << std::endl;
            }
            comState = AuthUI::ComState::NOTHING;
        }
    }
    UIModule::Update(timeElapsed);
}

void AuthUI::SetServerAnswer(bool result) {
    ui->Lock();
    serverAnswer = ServerAnswer(result);
    ui->Unlock();
}

void AuthUI::generateLoginWindow() {
    logWindow = new Container();
	logWindow->SetSize(sf::Vector2f(600, 103));
    logWindow->GetStyle().backgroundColor = sf::Color::Transparent;
	widgets.push_back(uptr<Container>(logWindow));

	Label *loginLabel = new Label(L"Login");
    loginLabel->GetStyle().textColor = sf::Color::White;
    loginLabel->GetStyle().fontSize = 16;
	logWindow->AddItem(loginLabel, sf::Vector2f(0, 0));

	my_login_entry = new Entry(sf::Vector2f(400, 25));
    my_login_entry->SetOnEnterFunc(std::bind(&AuthUI::login, this));
    my_login_entry->GetStyle().backgroundColor = sf::Color(60, 60, 60);
    my_login_entry->GetStyle().textColor = sf::Color(193, 205, 205);
    my_login_entry->GetStyle().fontSize = 16;
	logWindow->AddItem(my_login_entry, sf::Vector2f(100, 0));

	Label *passwordLabel = new Label(L"Password");
    passwordLabel->SetStyle(loginLabel->GetStyle());
	logWindow->AddItem(passwordLabel, sf::Vector2f(0, 26));

	my_passw_entry = new Entry(sf::Vector2f(400, 25));
    my_passw_entry->SetStyle(my_login_entry->GetStyle());
    my_passw_entry->HideSymbols();
    my_passw_entry->SetOnEnterFunc(std::bind(&AuthUI::login, this));
	logWindow->AddItem(my_passw_entry, sf::Vector2f(100, 26));

	Button *enterButton = new Button(L"Enter", sf::Vector2f(100, 51), std::bind(&AuthUI::login, this));
    enterButton->GetStyle().textColor = sf::Color::White;
    enterButton->GetStyle().fontSize = 16;
    logWindow->AddItem(enterButton, sf::Vector2f(500, 0));

	Button *regButton = new Button(L"Registration", sf::Vector2f(100, 51), std::bind(&AuthUI::openReg, this));
    regButton->SetStyle(enterButton->GetStyle());
    logWindow->AddItem(regButton, sf::Vector2f(500, 52));
}

void AuthUI::generateRegistrationWindow() {
    regWindow = new Container();
	regWindow->SetSize(sf::Vector2f(650, 72));
	regWindow->GetStyle().backgroundColor = sf::Color::Transparent;
	widgets.push_back(uptr<Container>(regWindow));

	Label *loginLabel = new Label(L"Login");
    loginLabel->GetStyle().fontSize = 16;
    regWindow->AddItem(loginLabel, sf::Vector2f(0, 21));

	my_new_login_entry = new Entry(sf::Vector2f(400, 25));
    my_new_login_entry->GetStyle().backgroundColor = sf::Color(193, 205, 205);
    my_new_login_entry->SetOnEnterFunc(std::bind(&AuthUI::registration, this));
	regWindow->AddItem(my_new_login_entry, sf::Vector2f(100, 21));

	Label *passwordLabel = new Label(L"Password");
    passwordLabel->SetStyle(loginLabel->GetStyle());
    regWindow->AddItem(passwordLabel, sf::Vector2f(0, 47));
    
	my_new_passw_entry = new Entry(sf::Vector2f(400, 25));
    my_new_passw_entry->SetStyle(my_new_login_entry->GetStyle());
    my_new_passw_entry->HideSymbols();
    my_new_passw_entry->SetOnEnterFunc(std::bind(&AuthUI::registration, this));
    regWindow->AddItem(my_new_passw_entry, sf::Vector2f(100, 47));

	Button *createAccountButton = new Button(L"Create account", sf::Vector2f(150, 51), std::bind(&AuthUI::registration, this));
    createAccountButton->GetStyle().textColor = sf::Color::White;
    createAccountButton->GetStyle().fontSize = 16;
    regWindow->AddItem(createAccountButton, sf::Vector2f(500, 21));

	Button *closeButton = new Button(L"Close", sf::Vector2f(100, 20), std::bind(&AuthUI::closeReg, this));
    closeButton->SetStyle(createAccountButton->GetStyle());
    regWindow->AddItem(closeButton, sf::Vector2f(500, 0));
}

void AuthUI::openReg() {
    curInputWidget = regWindow;
    regWindow->Show();
}

void AuthUI::closeReg() {
    regWindow->Hide();
	my_login_entry->Clear();
	my_passw_entry->Clear();
    curInputWidget = logWindow;
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

AuthUI::ServerAnswer AuthUI::getAnswer() {
	ui->Lock();
	ServerAnswer temp = serverAnswer;
	serverAnswer.isAnswer = false;
	ui->Unlock();
	return temp;
}