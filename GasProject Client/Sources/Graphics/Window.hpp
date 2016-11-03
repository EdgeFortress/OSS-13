#pragma once

#include <memory>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "TileGrid/TileGrid.hpp"
#include "UI/UI.hpp"

using sf::RenderWindow;

class Window {
private:
	uptr<TileGrid> tileGrid;
	uptr<UI> ui;

	list<uptr<Texture>> textures;

	uptr<RenderWindow> window;
	int width, height;

	const int req_FPS = 100;
	sf::Clock clock;
	int cur_FPS;
	void loadTextures();

	bool fps_exceed() {
		if (clock.getElapsedTime() >= sf::milliseconds(100)) {
			cur_FPS = 0;
			clock.restart();
		}
		if (cur_FPS >= req_FPS / 10) return true;
		return false;
	}

public:
	Window() : 
		tileGrid(new TileGrid(textures)),
		cur_FPS(0) 
	{
		loadTextures();
		sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
		width = static_cast<int>(0.9 * videoMode.width);
		height = static_cast<int>(0.9 * videoMode.height);
		window.reset(new RenderWindow(sf::VideoMode(width, height), "GasProjectClient"));
		window->clear(sf::Color::Black);
		window->display();
		ui.reset(new UI(window.get()));
	}

	void Initilize() {
		tileGrid->Initialize();
	}

	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;
	virtual ~Window() = default;

	void Update(sf::Time);

	bool isOpen() const { return window->isOpen(); }

	list<uptr<Texture>> &GetTextures() { return textures;  }

	UI *GetUI() const { return ui.get(); }
};