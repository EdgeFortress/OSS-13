#include "AuthUI.hpp"

#include <SFML/Graphics.hpp>
#include <typeinfo>

#include <Shared/Network/Protocol/ClientToServer/Commands.h>

#include "Graphics/UI/UI.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Network.hpp"

#include "Graphics/UI/Widget/Label.hpp"
#include "Graphics/UI/Widget/Button.hpp"
#include "Graphics/UI/Widget/Entry.hpp"

using namespace network::protocol;

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
}

void AuthUI::Update(sf::Time timeElapsed) {
    if (comState != AuthUI::ComState::NOTHING) {
        AuthUI::ServerAnswer answer = getAnswer();
        if (answer.isAnswer) {
            if (comState == AuthUI::ComState::LOGIN) {
                if (answer.result) {
                    LOGI << "Successfully logged in";
					auto *p = new client::JoinGameCommand();
					Connection::commandQueue.Push(p);
                    ui->ChangeModule<GameProcessUI>();
                }
                else {
                    LOGE << "Failed to log in";
                }
            }
            if (comState == AuthUI::ComState::REGISTRATION) {
                if (answer.result) {
					LOGI << "You are succesfully registered" << std::endl;
                    closeReg();
                }
                else
					LOGE << "Failed to registrate" << std::endl;
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
    logWindow->SetSize({600, 103});
    logWindow->GetStyle().backgroundColor = sf::Color::Transparent;
	widgets.push_back(uptr<Container>(logWindow));

	Label *loginLabel = new Label(L"Login");
    loginLabel->GetStyle().textColor = sf::Color::White;
    loginLabel->GetStyle().fontSize = 16;
    logWindow->AddItem(loginLabel, {0, 0});

    my_login_entry = new Entry({400, 25});
    my_login_entry->SetOnEnterFunc(std::bind(&AuthUI::login, this));
    my_login_entry->GetStyle().backgroundColor = sf::Color(60, 60, 60);
    my_login_entry->GetStyle().textColor = sf::Color(193, 205, 205);
    my_login_entry->GetStyle().fontSize = 16;
    logWindow->AddItem(my_login_entry, {100, 0});

	Label *passwordLabel = new Label(L"Password");
    passwordLabel->SetStyle(loginLabel->GetStyle());
    logWindow->AddItem(passwordLabel, {0, 26});

    my_passw_entry = new Entry({400, 25});
    my_passw_entry->SetStyle(my_login_entry->GetStyle());
    my_passw_entry->HideSymbols();
    my_passw_entry->SetOnEnterFunc(std::bind(&AuthUI::login, this));
    logWindow->AddItem(my_passw_entry, {100, 26});

    Style underCursorButtonStyle;
    underCursorButtonStyle.textColor = sf::Color::Black;
    underCursorButtonStyle.backgroundColor = sf::Color::White;
    underCursorButtonStyle.fontSize = 16;

    Button *enterButton = new Button(L"Enter", {100, 51}, std::bind(&AuthUI::login, this));
    enterButton->GetStyle().textColor = sf::Color::White;
    enterButton->GetStyle().fontSize = 16;
    enterButton->SetUnderCursorStyle(underCursorButtonStyle);
    logWindow->AddItem(enterButton, {500, 0});

    Button *regButton = new Button(L"Registration", {100, 51}, std::bind(&AuthUI::openReg, this));
    regButton->SetStyle(enterButton->GetStyle());
    regButton->SetUnderCursorStyle(underCursorButtonStyle);
    logWindow->AddItem(regButton, { 500, 52 });
    logWindow->SetActive(true);
}

void AuthUI::generateRegistrationWindow() {
    regWindow = new Container();
    regWindow->SetSize({650, 72});
	regWindow->GetStyle().backgroundColor = sf::Color::Transparent;
	widgets.push_back(uptr<Container>(regWindow));

	Label *loginLabel = new Label(L"Login");
    loginLabel->GetStyle().fontSize = 16;
    regWindow->AddItem(loginLabel, {0, 21});

    my_new_login_entry = new Entry({400, 25});
    my_new_login_entry->GetStyle().backgroundColor = sf::Color(60, 60, 60);
    my_new_login_entry->GetStyle().textColor = sf::Color(193, 205, 205);
    my_new_login_entry->GetStyle().fontSize = 16;
    my_new_login_entry->SetOnEnterFunc(std::bind(&AuthUI::registration, this));
    regWindow->AddItem(my_new_login_entry, {100, 21});

	Label *passwordLabel = new Label(L"Password");
    passwordLabel->SetStyle(loginLabel->GetStyle());
    regWindow->AddItem(passwordLabel, {0, 47});
    
    my_new_passw_entry = new Entry({400, 25});
    my_new_passw_entry->SetStyle(my_new_login_entry->GetStyle());
    my_new_passw_entry->HideSymbols();
    my_new_passw_entry->SetOnEnterFunc(std::bind(&AuthUI::registration, this));
    regWindow->AddItem(my_new_passw_entry, {100, 47});

    Button *createAccountButton = new Button(L"Create account", {150, 51}, std::bind(&AuthUI::registration, this));
    createAccountButton->GetStyle().textColor = sf::Color::White;
    createAccountButton->GetStyle().fontSize = 16;
    regWindow->AddItem(createAccountButton, {500, 21});

    Button *closeButton = new Button(L"Close", {100, 20}, std::bind(&AuthUI::closeReg, this));
    closeButton->SetStyle(createAccountButton->GetStyle());
    regWindow->AddItem(closeButton, {500, 0});
}

void AuthUI::openReg() {
    //curInputWidget = regWindow;
    SetCurActiveWidget(regWindow);
    regWindow->Show();
}

void AuthUI::closeReg() {
    regWindow->Hide();
	my_login_entry->Clear();
	my_passw_entry->Clear();
    //curInputWidget = logWindow;
    SetCurActiveWidget(logWindow);
}

void AuthUI::login() {
    if (comState == ComState::NOTHING) {
		auto *p = new client::AuthorizationCommand();
		p->login = my_login_entry->GetText();
		p->password = my_passw_entry->GetText();
        Connection::commandQueue.Push(p);
        comState = ComState::LOGIN;
    }
    else
        LOGW << "Wait for answer to previous command" << std::endl;
}

void AuthUI::registration() {
    if (comState == ComState::NOTHING) {
		auto *p = new client::RegistrationCommand();
		p->login = my_login_entry->GetText();
		p->password = my_passw_entry->GetText();
        Connection::commandQueue.Push(p);
        comState = ComState::REGISTRATION;
    }
    else
        LOGW << "Wait for answer to previous command" << std::endl;
}

AuthUI::ServerAnswer AuthUI::getAnswer() {
	ui->Lock();
	ServerAnswer temp = serverAnswer;
	serverAnswer.isAnswer = false;
	ui->Unlock();
	return temp;
}