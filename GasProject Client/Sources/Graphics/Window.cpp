#include "Window.hpp"

#include <fstream>

#include "Client.hpp"
#include "TileGrid.hpp"
#include "UI/UI.hpp"

#include "Shared/JSON.hpp"
#include "Shared/OS.hpp"

Window::Window() {
	ui.reset(new UI);
}

void Window::Initialize() {
    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
    width = int(0.8 * videoMode.width);
    height = int(0.8 * videoMode.height);
    window.reset(new sf::RenderWindow(sf::VideoMode(width, height), "GasProjectClient"));
    ui->ChangeModule<AuthUI>();
    resize(window->getSize().x, window->getSize().y);
}

void Window::Update() {
	// FPS Counter
	//
	////static int cur_FPS = 0;
	////cur_FPS++;
	////static sf::Time counter;
	////counter += lastFrameTime;
	////if (counter > sf::seconds(1)) {
	////	CC::log << "FPS" << cur_FPS << std::endl;
	////	cur_FPS = 0;
	////	counter = sf::Time::Zero;
	////}

	sf::Event event;

	while (window->pollEvent(event)) {
		ui->HandleEvent(event);
		if (event.type == sf::Event::Resized)
			resize(event.size.width, event.size.height);
		if (event.type == sf::Event::Closed)
			window->close();
	}

	window->clear(sf::Color::Black);
	ui->Update(lastFrameTime);
	ui->Draw(window.get());
	window->display();

	fps_sleep();
}

bool Window::isOpen() const { return window->isOpen(); }
int Window::GetWidth() const { return width; }
int Window::GetHeight() const { return height; }
sf::Vector2i Window::GetPosition() const { return window->getPosition(); }
UI *Window::GetUI() const { return ui.get(); }

void Window::fps_sleep() {
    sf::Time timeElapsed = frame_clock.getElapsedTime();

    if (timeElapsed.asMicroseconds() < 1000 * 1000 / req_FPS)
        sf::sleep(sf::microseconds(1000 * 1000 / req_FPS) - timeElapsed);

    lastFrameTime = frame_clock.restart();
}

void Window::resize(const int newWidth, const int newHeight) {
    width = newWidth; height = newHeight;
    const sf::FloatRect visibleArea(0, 0, float(width), float(height));
    window->setView(sf::View(visibleArea));
    ui->Resize(width, height);
}