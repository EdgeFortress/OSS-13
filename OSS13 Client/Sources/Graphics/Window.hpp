#pragma once

#include <SFML/System.hpp>

#include "Shared/Types.hpp"

class UI;

namespace sf {
	class RenderWindow;
}

class Window {
public:
    Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    virtual ~Window() = default;

    void Initialize();
    void Update();

    bool isOpen() const;
    int GetWidth() const;
    int GetHeight() const;
    sf::Vector2i GetPosition() const;
    UI *GetUI() const;

private:
    uptr<UI> ui;

    uptr<sf::RenderWindow> window;
    uf::vec2i resolution;

    const int req_FPS = 60;
    sf::Clock frame_clock;
	sf::Time lastFrameTime;

	// sleep, if frame was drawed so fast
	void fps_sleep();

    void resize(const int newWidth, const int newHeight);
};