#pragma once

#include "UI.hpp"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

using std::cout;
using std::endl;
using std::string;

void UI::Enter() {
	bool enter;

	cout << string(login_entry->GetText()) << endl;
	cout << string(passw_entry->GetText()) << endl;

	if (login_entry->GetText() == "abc" && passw_entry->GetText() == "123")
		enter = true;
	else
		enter = false;

	if (enter)
		cout << "Enter succeeded!" << endl;
	else
		cout << "Enter failed!" << endl;
}
void UI::Registration() {
	cout << "You have just created new login data:" << endl;
	cout << string(new_login_entry->GetText()) << endl;
	cout << string(new_passw_entry->GetText()) << endl;
}
void UI::HandleEvent(sf::Event event) {
	desktop->HandleEvent(event);
}
void UI::run_login() {
	auto login_label = sfg::Label::Create("Login");
	auto passw_label = sfg::Label::Create("Password");

	auto enter_button = sfg::Button::Create("Enter");
	auto reg_button = sfg::Button::Create("Registration");
	enter_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&UI::Enter, this));
	reg_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&UI::run_reg, this));

	login_entry = sfg::Entry::Create();
	passw_entry = sfg::Entry::Create();
	login_entry->SetRequisition(sf::Vector2f(100.f, 0.f));
	passw_entry->SetRequisition(sf::Vector2f(100.f, 0.f));

	auto table = sfg::Table::Create();
	table->Attach(login_label, sf::Rect<unsigned>(0, 0, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));
	table->Attach(passw_label, sf::Rect<unsigned>(0, 5, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));
	table->Attach(reg_button, sf::Rect<unsigned>(0, 10, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));
	table->Attach(login_entry, sf::Rect<unsigned>(5, 0, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));
	table->Attach(passw_entry, sf::Rect<unsigned>(5, 5, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));
	table->Attach(enter_button, sf::Rect<unsigned>(5, 10, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));

	auto window = sfg::Window::Create();
	reg_window = sfg::Window::Create();
	window->Add(table);

	desktop = new sfg::Desktop;
	desktop->Add(window);
}
void UI::run_reg() {
	auto login_label = sfg::Label::Create("Input login");
	auto passw_label = sfg::Label::Create("Input password");

	auto reg_button = sfg::Button::Create("Registration");
	reg_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&UI::Registration, this));

	new_login_entry = sfg::Entry::Create();
	new_passw_entry = sfg::Entry::Create();
	new_login_entry->SetRequisition(sf::Vector2f(100.f, 0.f));
	new_passw_entry->SetRequisition(sf::Vector2f(100.f, 0.f));

	auto table = sfg::Table::Create();
	table->Attach(login_label, sf::Rect<unsigned>(0, 0, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));
	table->Attach(passw_label, sf::Rect<unsigned>(0, 5, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));
	table->Attach(reg_button, sf::Rect<unsigned>(3, 10, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));
	table->Attach(new_login_entry, sf::Rect<unsigned>(5, 0, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));
	table->Attach(new_passw_entry, sf::Rect<unsigned>(5, 5, 5, 5), sfg::Table::AttachOption::FILL,
		sfg::Table::AttachOption::FILL, sf::Vector2f(5.f, 1.f));

	reg_window->Add(table);

	desktop->Add(reg_window);
}