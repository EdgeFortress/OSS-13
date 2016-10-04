#include <SFML/Graphics.hpp>

#include <SFGUI\SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class Client {

};

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

	auto m_label = sfg::Label::Create("Hello world!");

	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}