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
    CC::log << "ButtonPressed" << endl;
}

void AuthUI::generateLoginWindow() {
    //auto login_label = sfg::Label::Create("Login");
    //auto passw_label = sfg::Label::Create("Password");
    //auto reg_label = sfg::Label::Create("Registration");
    //reg_label->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&AuthUI::openReg, this));

    //auto enter_button = sfg::Button::Create("Enter");
    //enter_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&AuthUI::login, this));

    //login_entry = sfg::Entry::Create();
    //passw_entry = sfg::Entry::Create();
    //login_entry->SetRequisition(sf::Vector2f(100, 0));
    //passw_entry->SetRequisition(sf::Vector2f(100, 0));

    //passw_entry->HideText('*');

    //auto table = sfg::Table::Create();

    //table->Attach(login_label,
    //    sf::Rect<unsigned>(0, 0, 2, 1),
    //    NULL,
    //    sfg::Table::AttachOption::FILL,
    //    sf::Vector2f(5, 0));
    //table->Attach(passw_label,
    //    sf::Rect<unsigned>(0, 2, 2, 1),
    //    NULL,
    //    sfg::Table::AttachOption::FILL,
    //    sf::Vector2f(5, 0));
    //table->Attach(login_entry,
    //    sf::Rect<unsigned>(2, 0, 3, 1),
    //    sfg::Table::AttachOption::FILL,
    //    NULL,
    //    sf::Vector2f(5, 2));
    //table->Attach(passw_entry,
    //    sf::Rect<unsigned>(2, 2, 3, 1),
    //    sfg::Table::AttachOption::FILL,
    //    NULL,
    //    sf::Vector2f(5, 2));
    //table->Attach(enter_button,
    //    sf::Rect<unsigned>(3, 3, 2, 1),
    //    sfg::Table::AttachOption::FILL,
    //    NULL,
    //    sf::Vector2f(0, 0));
    //table->Attach(reg_label,
    //    sf::Rect<unsigned>(5, 3, 1, 1),
    //    sfg::Table::AttachOption::FILL,
    //    sfg::Table::AttachOption::FILL,
    //    sf::Vector2f(5, 0));

    //logWindow = sfg::Window::Create();
    //logWindow->Add(table);
    //logWindow->SetPosition(sf::Vector2f((ui->GetRenderWindow()->getSize().x - logWindow->GetAllocation().width) / 10,
        //(ui->GetRenderWindow()->getSize().y - logWindow->GetAllocation().height) / 3));
    //logWindow->SetStyle(NULL);
    //ui->GetDesktop()->Add(logWindow);
    
    //uptr<Container> container;
	//container.reset(new Container(sf::Vector2f(100, 50)));
	//container->AddItem(new Label(L"Логин", ui->GetFont(), 16), sf::Vector2f(0, 0));
    //container->SetPosition(100, 100);
    //container->SetBackground(sf::Color::Green);
    //widgets.push_back(move(container));

    //container.reset(new Container(sf::Vector2f(100, 70)));
    //container->AddItem(new Button(L"Войти", sf::Vector2f(80, 50), sf::Color::Blue, ui->GetFont(), std::function<void()>(OnButtonClick)), sf::Vector2f(0, 0));
    //container->SetPosition(400, 400);
    //container->SetBackground(sf::Color::Red);
    //widgets.push_back(move(container));

    my_login_entry = new Entry(sf::Vector2f(400, 25), sf::Color(193, 205, 205), ui->GetFont());
    my_passw_entry = new Entry(sf::Vector2f(400, 25), sf::Color(193, 205, 205), ui->GetFont(), true);
    logWindow = new Container(sf::Vector2f(600, 103));
    logWindow->AddItem(new Label(L"Логин", ui->GetFont(), 16), sf::Vector2f(0, 0));
    logWindow->AddItem(new Label(L"Пароль", ui->GetFont(), 16), sf::Vector2f(0, 26));
    logWindow->AddItem(my_login_entry, sf::Vector2f(100, 0));
    logWindow->AddItem(my_passw_entry, sf::Vector2f(100, 26));
    logWindow->AddItem(new Button(L"Войти", sf::Vector2f(100, 51), sf::Color::White, ui->GetFont(), std::bind(&AuthUI::login, this)), sf::Vector2f(500, 0));
    logWindow->AddItem(new Button(L"Регистрация", sf::Vector2f(100, 51), sf::Color::White, ui->GetFont(), std::bind(&AuthUI::openReg, this)), sf::Vector2f(500, 52));
    logWindow->SetPosition(200, 200);
    logWindow->SetBackground(sf::Color::Transparent);
    widgets.push_back(uptr<Container>(logWindow));

    my_login_entry->GrabFocus();
}

void AuthUI::generateRegistrationWindow() {
    //auto designation_label = sfg::Label::Create("Fill the form:");
    //auto login_label = sfg::Label::Create("Login:");
    //auto passw_label = sfg::Label::Create("Password:");

    //auto reg_button = sfg::Button::Create("Create new account");
    //reg_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&AuthUI::registration, this));

    //new_login_entry = sfg::Entry::Create();
    //new_passw_entry = sfg::Entry::Create();
    //new_login_entry->SetRequisition(sf::Vector2f(100.f, 0.f));
    //new_passw_entry->SetRequisition(sf::Vector2f(100.f, 0.f));

    //new_passw_entry->HideText('*');

    //auto table = sfg::Table::Create();
    //table->Attach(login_label,
    //    sf::Rect<unsigned>(0, 0, 5, 5),
    //    sfg::Table::AttachOption::FILL,
    //    sfg::Table::AttachOption::FILL,
    //    sf::Vector2f(5.f, 1.f));
    //table->Attach(passw_label,
    //    sf::Rect<unsigned>(0, 5, 5, 5),
    //    sfg::Table::AttachOption::FILL,
    //    sfg::Table::AttachOption::FILL,
    //    sf::Vector2f(5.f, 1.f));
    //table->Attach(reg_button,
    //    sf::Rect<unsigned>(3, 10, 5, 5),
    //    sfg::Table::AttachOption::FILL,
    //    sfg::Table::AttachOption::FILL,
    //    sf::Vector2f(5.f, 1.f));
    //table->Attach(new_login_entry,
    //    sf::Rect<unsigned>(5, 0, 5, 5),
    //    sfg::Table::AttachOption::FILL,
    //    sfg::Table::AttachOption::FILL,
    //    sf::Vector2f(5.f, 1.f));
    //table->Attach(new_passw_entry,
    //    sf::Rect<unsigned>(5, 5, 5, 5),
    //    sfg::Table::AttachOption::FILL,
    //    sfg::Table::AttachOption::FILL,
    //    sf::Vector2f(5.f, 1.f));

    //regWindow = sfg::Window::Create();
    //regWindow->Add(table);
    //regWindow->SetStyle(sfg::Window::Style::TOPLEVEL ^ sfg::Window::Style::RESIZE | sfg::Window::Style::CLOSE);
    //regWindow->SetTitle("Registration of new account");
    //regWindow->Show(false);
    //regWindow->GetSignal(sfg::Window::OnCloseButton).Connect(std::bind(&AuthUI::closeReg, this));
    //ui->GetDesktop()->Add(regWindow);

    my_new_login_entry = new Entry(sf::Vector2f(400, 25), sf::Color(193, 205, 205), ui->GetFont());
    my_new_passw_entry = new Entry(sf::Vector2f(400, 25), sf::Color(193, 205, 205), ui->GetFont(), true);
    regWindow = new Container(sf::Vector2f(650, 72));
    regWindow->AddItem(new Label(L"Логин", ui->GetFont(), 16), sf::Vector2f(0, 21));
    regWindow->AddItem(new Label(L"Пароль", ui->GetFont(), 16), sf::Vector2f(0, 47));
    regWindow->AddItem(my_new_login_entry, sf::Vector2f(100, 21));
    regWindow->AddItem(my_new_passw_entry, sf::Vector2f(100, 47));
    regWindow->AddItem(new Button(L"Создать аккаунт", sf::Vector2f(150, 51), sf::Color::White, ui->GetFont(), std::bind(&AuthUI::registration, this)), sf::Vector2f(500, 21));
    regWindow->AddItem(new Button(L"Закрыть", sf::Vector2f(100, 20), sf::Color::White, ui->GetFont(), std::bind(&AuthUI::closeReg, this)), sf::Vector2f(500, 0));
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
        CC::log << "Wait for answer to previous command" << endl;
}

void AuthUI::registration() {
    if (comState == ComState::NOTHING) {
        Connection::commandQueue.Push(new RegistrationClientCommand(my_new_login_entry->GetText(), my_new_passw_entry->GetText()));
        comState = ComState::REGISTRATION;
    }
    else
        CC::log << "Wait for answer to previous command" << endl;
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