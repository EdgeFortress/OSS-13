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
    Connection::commandQueue.Push(new AuthorizationClientCommand(login_entry->GetText(), passw_entry->GetText()));
    comState = ComState::LOGIN;
}

void AuthUI::registration() {
	Connection::commandQueue.Push(new RegistrationClientCommand(new_login_entry->GetText(), new_passw_entry->GetText()));
    comState = ComState::REGISTRATION;
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

void AuthUI::AccountDataEnter() {
	if (login_entry->HasFocus() || passw_entry->HasFocus())
		this->login();
	if (new_login_entry->HasFocus() || new_passw_entry->HasFocus())
		this->registration();
}

GameRow::GameRow(int id, string title, int num_of_players) :
    id(id),
    box(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 0))
{ 
    auto icon = sfg::Button::Create();
    icon->SetRequisition(sf::Vector2f(32, 32));
    box->Pack(icon, false, false);

    auto title_label = sfg::Label::Create(title);
    title_label->SetAlignment(sf::Vector2f(0.05f, 0.5f));
    box->Pack(title_label, true, true);

    box->Pack(sfg::Separator::Create(sfg::Separator::Orientation::VERTICAL), false, false);

    auto space = sfg::Label::Create();
    space->SetRequisition(sf::Vector2f(10, 0));
    box->Pack(space, false, false);

    auto num_of_players_label = sfg::Label::Create(std::to_string(num_of_players));
    num_of_players_label->SetRequisition(sf::Vector2f(35, 0));
    box->Pack(num_of_players_label, false, false);
    
    space = sfg::Label::Create();
    space->SetRequisition(sf::Vector2f(20, 0));
    box->Pack(space, false, false);

    auto join_button = sfg::Button::Create("join");
    join_button->SetRequisition(sf::Vector2f(50, 32));
    join_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&GameRow::join, this));
    box->Pack(join_button, false, true);
}

void GameRow::join() {
    Connection::commandQueue.Push(new JoinGameClientCommand(id));
}

GameListUI::GameListUI(UI *ui) : UIModule(ui) {
    generateGamelistWindow();
    Hide();
}

void GameListUI::generateGamelistWindow() {
    auto designation_label = sfg::Label::Create("Games");
    designation_label->SetAlignment(sf::Vector2f(0, 0.5));

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
    box->Pack(designation_label, false, false);

    auto frame = sfg::Frame::Create();
    gamesBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 0);
    frame->Add(gamesBox);
    box->Pack(frame);

    auto updateBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 0);
    auto updateButton = sfg::Button::Create("Update");
    updateButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&GameListUI::update, this));
    auto updateSpaceLabel = sfg::Label::Create("");
    updateSpaceLabel->SetRequisition(sf::Vector2f(1, 0));
    updateBox->Pack(updateSpaceLabel);
    updateBox->Pack(updateButton, false, false);
    box->Pack(updateBox, false, false);

    gamelistWindow = sfg::Window::Create();
    gamelistWindow->Add(box);
    gamelistWindow->SetRequisition(sf::Vector2f(static_cast<float>(ui->GetRenderWindow()->getSize().x) / 2, 
                                                static_cast<float>(ui->GetRenderWindow()->getSize().y) / 2));
    gamelistWindow->SetPosition(sf::Vector2f((ui->GetRenderWindow()->getSize().x - gamelistWindow->GetAllocation().width) / 2,
                                             (ui->GetRenderWindow()->getSize().y - gamelistWindow->GetAllocation().height) / 2));
    gamelistWindow->SetStyle(sfg::Window::Style::BACKGROUND);
    ui->GetDesktop()->Add(gamelistWindow);
}

void GameListUI::update() {
    Connection::commandQueue.Push(new GameListClientRequest());
}

void GameListUI::AddGame(int id, string title, int num_of_players) {
    games.push_back(uptr<GameRow>(new GameRow(id, title, num_of_players)));
    gamesBox->Pack(sfg::Separator::Create(sfg::Separator::Orientation::HORIZONTAL), false, false);
    gamesBox->Pack(games.back()->box, false, true);
    gamesBox->Pack(sfg::Separator::Create(sfg::Separator::Orientation::HORIZONTAL), false, false);
}

void GameListUI::Clear() {
    games.clear();
    gamesBox->RemoveAll();
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

void UI::Update(sf::Time timeElapsed) {
    desktop.Update(timeElapsed.asSeconds());
}

void UI::Draw(sf::RenderWindow *render_window) {
    m_sfgui.Display(*render_window);
}

void UI::Lock() {
    UImutex.lock();
};

void UI::Unlock() {
    UImutex.unlock();
}
