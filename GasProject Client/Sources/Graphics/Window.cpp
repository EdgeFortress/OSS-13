#include "Window.hpp"

#include "Client.hpp"
#include "TileGrid.hpp"
#include "UI/UI.hpp"

void Window::fps_sleep() {
	sf::Time timeElapsed = frame_clock.getElapsedTime();

	if (timeElapsed.asMicroseconds() < 1000 * 1000 / req_FPS)
		sf::sleep(sf::microseconds(1000 * 1000 / req_FPS) - timeElapsed);

	lastFrameTime = frame_clock.restart();
}

void Window::resize(const int newWidth, const int newHeight) {
	width = newWidth; height = newHeight;
	sf::FloatRect visibleArea(0, 0, float(width), float(height));
	window->setView(sf::View(visibleArea));
	ui->Resize(width, height);
}

Window::Window() :
	animationTime(sf::Time::Zero)
{
	loadTextures(textures, sprites);

	ui.reset(new UI);
}

void Window::Initialize() {
	sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	width = int(0.8 * videoMode.width);
	height = int(0.8 * videoMode.height);
	window.reset(new sf::RenderWindow(sf::VideoMode(width, height), "GasProjectClient"));
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

	animationTime += lastFrameTime;
	if (animationTime >= sf::seconds(1)) {
		animationTime = sf::Time::Zero;
		for (auto &sprite : sprites)
			if (sprite->Animated())
				sprite->UpdateFrame();
	}

	window->clear(sf::Color::Black);
	ui->Update(lastFrameTime);
	ui->Draw(window.get());
	window->display();

	fps_sleep();
}

void Window::SetSpriteSize(uint size) {
	for (auto &sprite : sprites)
		sprite->Resize(size);
}

std::list<uptr<Sprite>> &Window::GetSprites() { return sprites; }

bool Window::isOpen() const { return window->isOpen(); }
int Window::GetWidth() const { return width; }
int Window::GetHeight() const { return height; }
sf::Vector2i Window::GetPosition() const { return window->getPosition(); }
UI *Window::GetUI() const { return ui.get(); }

void Window::loadTextures(std::list<uptr<Texture>> &textures, std::list<uptr<Sprite>> &sprites) {
	Texture *t = new Texture("Images/Human.png", 32, 1);
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Mob, 0, 0, true, 1)));
	textures.push_back(uptr<Texture>(t));
	t = new Texture("Images/Turfs.png", 32, 2);
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Wall, 0, 0, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Floor, 1, 1, false, 1)));
	textures.push_back(uptr<Texture>(t));
	t = new Texture("Images/Ghost.png", 32, 1);
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Ghost, 0, 0, false, 1)));
	t = new Texture("Images/Space.png", 32, 26);
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite::Space, 0, 0, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 1), 1, 1, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 2), 2, 2, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 3), 3, 3, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 4), 4, 4, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 5), 5, 5, false, 5)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 6), 6, 10, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 7), 7, 14, false, 5)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 8), 8, 19, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 9), 9, 23, false, 5)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 10), 10, 28, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 11), 11, 32, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 12), 12, 36, false, 5)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 13), 13, 41, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 14), 14, 45, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 15), 15, 49, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 16), 16, 53, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 17), 17, 57, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 18), 18, 61, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 19), 19, 65, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 20), 20, 69, false, 4)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 21), 21, 73, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 22), 22, 74, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 23), 23, 75, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 24), 24, 76, false, 1)));
	sprites.push_back(uptr<Sprite>(t->SetInfo(Global::Sprite(int(Global::Sprite::Space) + 25), 25, 77, false, 1)));
	textures.push_back(uptr<Texture>(t));
}