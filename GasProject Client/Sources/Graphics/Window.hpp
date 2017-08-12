#pragma once

#include <list>
#include <SFML/System.hpp>

#include "Shared/Types.hpp"

class UI;
class TileGrid;
class Sprite;
class Texture;

namespace sf {
	class RenderWindow;
}

class Window {
private:
    uptr<UI> ui;

    std::list<uptr<Texture>> textures;
    std::list<uptr<Sprite>> sprites;

    uptr<sf::RenderWindow> window;
    int width, height;

    sf::Time animationTime;

    const int req_FPS = 60;
    sf::Clock frame_clock;
	sf::Time lastFrameTime;

	// sleep, if frame was drawed so fast
	void fps_sleep();

    void resize(const int newWidth, const int newHeight);
    void loadTextures(std::list<uptr<Texture>> &, std::list<uptr<Sprite>> &);

public:
	Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    virtual ~Window() = default;

    void Initialize();
    void Update();

	void SetSpriteSize(uint size);

	// TODO: Remove this
	std::list<uptr<Sprite>> &GetSprites();

	bool isOpen() const;
	int GetWidth() const;
	int GetHeight() const;
	sf::Vector2i GetPosition() const;
	UI *GetUI() const;
};