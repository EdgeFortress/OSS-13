#pragma once

#include <SFML/System.hpp>

#include <Shared/Types.hpp>
#include <Browser/BrowserController.h>

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

    void Initialize(const std::filesystem::path& executablePath);
    void Update();

    bool isOpen() const;
    int GetWidth() const;
    int GetHeight() const;
    uf::vec2i GetPosition() const;
    BrowserController *GetBrowserController() const;
    UI *GetUI() const;
    sf::WindowHandle GetSystemHandle() const;

private:
    // sleep, if frame was drawed so fast
    void fps_sleep();
    void resize(const int newWidth, const int newHeight);

    void onClose();

private:
    uptr<BrowserController> browserController;
    uptr<UI> ui;

    uptr<sf::RenderWindow> window;
    uf::vec2i resolution;
    uf::vec2i viewPosition;

    const int req_FPS = 60;
    sf::Clock frame_clock;
    sf::Time lastFrameTime;

    bool isClosing{};
};
