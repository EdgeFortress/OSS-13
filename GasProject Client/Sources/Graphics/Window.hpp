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
    list<uptr<Sprite>> sprites;

    uptr<RenderWindow> window;
    int width, height;

    sf::Time animationTime;

    const int req_FPS = 100;
    sf::Clock clock;
    int cur_FPS;

    bool fps_exceed() {
        if (clock.getElapsedTime() >= sf::milliseconds(100)) {
            cur_FPS = 0;
            clock.restart();
        }
        if (cur_FPS >= req_FPS / 10) return true;
        return false;
    }

    void loadTextures(list<uptr<Texture>> &, list<uptr<Sprite>> &);

public:
    Window() : 
        cur_FPS(0) ,
        animationTime(sf::Time::Zero)
    {
        loadTextures(textures, sprites);

        tileGrid.reset(new TileGrid);
        ui.reset(new UI);

        sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
        width = static_cast<int>(0.9 * videoMode.width);
        height = static_cast<int>(0.9 * videoMode.height);
        window.reset(new RenderWindow(sf::VideoMode(width, height), "GasProjectClient"));
        Resize(width, height);
    }

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    virtual ~Window() = default;

    void Update(sf::Time);
    void Resize(const int newWidth, const int newHeight);

    bool isOpen() const { return window->isOpen(); }

    //list<uptr<Texture>> &GetTextures() { return textures;  }
    list<uptr<Sprite>> &GetSprites() { return sprites; }

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    UI *GetUI() const { return ui.get(); }
    TileGrid *GetTileGrid() const { return tileGrid.get(); }
};