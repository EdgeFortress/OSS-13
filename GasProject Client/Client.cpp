#include <SFML/Graphics.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class Client {

};

sfg::Label::Ptr m_label;

void click()
{
	m_label->SetText("Hello SFGUI, pleased to meet you!");
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 500), "SFML works!");

	sfg::SFGUI m_sfgui;
	m_label = sfg::Label::Create("Hello world!");

	auto button = sfg::Button::Create("Greet_SFGUI!");
	button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(click));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.f);
	box->Pack(m_label);
	box->Pack(button, true);

	auto new_window = sfg::Window::Create();
	new_window->SetTitle("BEST_GUI!");
	new_window->Add(box);

	sfg::Desktop desktop;
	desktop.Add(new_window);

	sf::Clock clock;

	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	window.resetGLStates();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			desktop.HandleEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		desktop.Update(clock.restart().asSeconds());
		window.clear();
		//window.draw(shape);
		m_sfgui.Display(window);
		window.display();
	}

	return 0;
}
