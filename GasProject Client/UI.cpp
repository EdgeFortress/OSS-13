#include <string>
#include <cstring>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Client.hpp"
#include "UI.hpp"

using std::string; 

UIModule::UIModule(UI *ui) : ui(ui) {

}

AuthUI::AuthUI(UI *ui) : UIModule(ui) {
    comState = NOTHING;
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
	ui->GetDesktop()->Add(regWindow);
}

void AuthUI::openLogin() {
	regWindow->Show(false);
	new_login_entry->SetText("");
	new_passw_entry->SetText("");
	logWindow->Show(true);
}

void AuthUI::openReg() {
	login_entry->SetText("");
	passw_entry->SetText("");
	regWindow->Show(true);
}

void AuthUI::closeReg() {
	regWindow->Show(false);
}

void AuthUI::login() {
    Connection::commandQueue.Push(new AuthorizationClientCommand(login_entry->GetText(), passw_entry->GetText()));
    comState = LOGIN;
}

void AuthUI::registration() {
	Connection::commandQueue.Push(new RegistrationClientCommand(new_login_entry->GetText(), new_passw_entry->GetText()));
    comState = REGISTRATION;
}

void AuthUI::Show() {
    openLogin();
}

void AuthUI::Hide() {
    regWindow->Show(false);
    logWindow->Show(false);
}

GameRow::GameRow(int id, string &title, int num_of_players) :
    id(id),
    title_label(sfg::Label::Create(title)),
    num_of_players_label(sfg::Label::Create(std::to_string(num_of_players)))
{ 
    
}

GameListUI::GameListUI(UI *ui) : UIModule(ui) {
    generateGamelistWindow();
    Hide();
}

void GameListUI::generateGamelistWindow() {
    auto designation_label = sfg::Label::Create("Choose the game:");

    auto table = sfg::Table::Create();
    table->Attach(designation_label,
        sf::Rect<unsigned>(0, 0, 5, 5),
        sfg::Table::AttachOption::FILL,
        sfg::Table::AttachOption::FILL,
        sf::Vector2f(5.f, 1.f));

    gamelistWindow = sfg::Window::Create();
    gamelistWindow->Add(table);
    gamelistWindow->SetPosition(sf::Vector2f((ui->GetRenderWindow()->getSize().x - gamelistWindow->GetAllocation().width) / 2,
                                             (ui->GetRenderWindow()->getSize().y - gamelistWindow->GetAllocation().height) / 2));
    gamelistWindow->SetStyle(sfg::Window::Style::BACKGROUND);
    ui->GetDesktop()->Add(gamelistWindow);
}

void GameListUI::Show() {
    gamelistWindow->Show(true);
}

void GameListUI::Hide() {
    gamelistWindow->Show(false);
}

UI::UI(sf::RenderWindow *rendWindow) :
	rendWindow(rendWindow),
	authUI(this),
    gamelistUI(this) {
	
}

void UI::HandleEvent(sf::Event event) {
	desktop.HandleEvent(event);
}
