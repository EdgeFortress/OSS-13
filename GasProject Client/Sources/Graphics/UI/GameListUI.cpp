#include "GameListUI.hpp"

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
