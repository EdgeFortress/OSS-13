#include "AuthUI.hpp"

void AuthUI::AccountDataEnter() {
	if (login_entry->HasFocus() || passw_entry->HasFocus())
		this->login();
	if (new_login_entry->HasFocus() || new_passw_entry->HasFocus())
		this->registration();
}


AuthUI::AuthUI(UI *ui) : UIModule(ui) {
	comState = ComState::NOTHING;
	generateLoginWindow();
	generateRegistrationWindow();
	Hide();
}

void AuthUI::generateLoginWindow() {
	auto login_label = sfg::Label::Create("Login");
	auto passw_label = sfg::Label::Create("Password");
	auto reg_label = sfg::Label::Create("Registration");
	reg_label->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&AuthUI::openReg, this));

	auto enter_button = sfg::Button::Create("Enter");
	enter_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&AuthUI::login, this));

	login_entry = sfg::Entry::Create();
	passw_entry = sfg::Entry::Create();
	login_entry->SetRequisition(sf::Vector2f(100, 0));
	passw_entry->SetRequisition(sf::Vector2f(100, 0));

	passw_entry->HideText('*');

	auto table = sfg::Table::Create();

	table->Attach(login_label,
		sf::Rect<unsigned>(0, 0, 2, 1),
		NULL,
		sfg::Table::AttachOption::FILL,
		sf::Vector2f(5, 0));
	table->Attach(passw_label,
		sf::Rect<unsigned>(0, 2, 2, 1),
		NULL,
		sfg::Table::AttachOption::FILL,
		sf::Vector2f(5, 0));
	table->Attach(login_entry,
		sf::Rect<unsigned>(2, 0, 3, 1),
		sfg::Table::AttachOption::FILL,
		NULL,
		sf::Vector2f(5, 2));
	table->Attach(passw_entry,
		sf::Rect<unsigned>(2, 2, 3, 1),
		sfg::Table::AttachOption::FILL,
		NULL,
		sf::Vector2f(5, 2));
	table->Attach(enter_button,
		sf::Rect<unsigned>(3, 3, 2, 1),
		sfg::Table::AttachOption::FILL,
		NULL,
		sf::Vector2f(0, 0));
	table->Attach(reg_label,
		sf::Rect<unsigned>(5, 3, 1, 1),
		sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL,
		sf::Vector2f(5, 0));

	logWindow = sfg::Window::Create();
	logWindow->Add(table);
	logWindow->SetPosition(sf::Vector2f((ui->GetRenderWindow()->getSize().x - logWindow->GetAllocation().width) / 10,
		(ui->GetRenderWindow()->getSize().y - logWindow->GetAllocation().height) / 3));
	logWindow->SetStyle(NULL);
	ui->GetDesktop()->Add(logWindow);
}

void AuthUI::generateRegistrationWindow() {
	auto designation_label = sfg::Label::Create("Fill the form:");
	auto login_label = sfg::Label::Create("Login:");
	auto passw_label = sfg::Label::Create("Password:");

	auto reg_button = sfg::Button::Create("Create new account");
	reg_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&AuthUI::registration, this));

	new_login_entry = sfg::Entry::Create();
	new_passw_entry = sfg::Entry::Create();
	new_login_entry->SetRequisition(sf::Vector2f(100.f, 0.f));
	new_passw_entry->SetRequisition(sf::Vector2f(100.f, 0.f));

	new_passw_entry->HideText('*');

	auto table = sfg::Table::Create();
	table->Attach(login_label,
		sf::Rect<unsigned>(0, 0, 5, 5),
		sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL,
		sf::Vector2f(5.f, 1.f));
	table->Attach(passw_label,
		sf::Rect<unsigned>(0, 5, 5, 5),
		sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL,
		sf::Vector2f(5.f, 1.f));
	table->Attach(reg_button,
		sf::Rect<unsigned>(3, 10, 5, 5),
		sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL,
		sf::Vector2f(5.f, 1.f));
	table->Attach(new_login_entry,
		sf::Rect<unsigned>(5, 0, 5, 5),
		sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL,
		sf::Vector2f(5.f, 1.f));
	table->Attach(new_passw_entry,
		sf::Rect<unsigned>(5, 5, 5, 5),
		sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL,
		sf::Vector2f(5.f, 1.f));

	regWindow = sfg::Window::Create();
	regWindow->Add(table);
	regWindow->SetPosition(sf::Vector2f((ui->GetRenderWindow()->getSize().x - regWindow->GetAllocation().width) / 2,
		(ui->GetRenderWindow()->getSize().y - regWindow->GetAllocation().height) / 2));
	regWindow->SetStyle(sfg::Window::Style::TOPLEVEL ^ sfg::Window::Style::RESIZE | sfg::Window::Style::CLOSE);
	regWindow->SetTitle("Registration of new account");
	regWindow->Show(false);
	regWindow->GetSignal(sfg::Window::OnCloseButton).Connect(std::bind(&AuthUI::closeReg, this));
	ui->GetDesktop()->Add(regWindow);
}

void AuthUI::openLogin() {
	regWindow->Show(false);
	new_login_entry->SetText("");
	new_passw_entry->SetText("");
	login_entry->GrabFocus();
	logWindow->Show(true);
}

void AuthUI::openReg() {
	login_entry->SetText("");
	passw_entry->SetText("");
	new_login_entry->GrabFocus();
	regWindow->Show(true);
}

void AuthUI::closeReg() {
	regWindow->Show(false);
}

void AuthUI::login() {
	if (comState == ComState::NOTHING) {
		Connection::commandQueue.Push(new AuthorizationClientCommand(login_entry->GetText(), passw_entry->GetText()));
		comState = ComState::LOGIN;
	}
	else
		CC::log << "Wait for answer to previous command" << endl;
}

void AuthUI::registration() {
	if (comState == ComState::NOTHING) {
		Connection::commandQueue.Push(new RegistrationClientCommand(new_login_entry->GetText(), new_passw_entry->GetText()));
		comState = ComState::REGISTRATION;
	}
	else
		CC::log << "Wait for answer to previous command" << endl;
}

void AuthUI::Show() {
	openLogin();
}

void AuthUI::Hide() {
	regWindow->Show(false);
	logWindow->Show(false);
}

void AuthUI::ChangeFocus() {
	if (login_entry->HasFocus())
		passw_entry->GrabFocus();
	else if (passw_entry->HasFocus())
		login_entry->GrabFocus();
	if (new_login_entry->HasFocus())
		new_passw_entry->GrabFocus();
	else if (new_passw_entry->HasFocus())
		new_login_entry->GrabFocus();
}