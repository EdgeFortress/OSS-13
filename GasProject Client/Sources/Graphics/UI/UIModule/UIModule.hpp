#pragma once

#include <list>

#include <Shared/Types.hpp>
#include <Shared/Network/Protocol/ServerToClient/WindowData.h>

class UI;
class Widget;
class CustomWidget;

namespace sf {
	class RenderWindow;
	class Time;
	class Event;
}

class UIModule {
public:
    UIModule(UI *ui);
    UIModule(const UIModule &) = delete;
    UIModule &operator=(const UIModule &) = delete;
    virtual ~UIModule() = default;

    virtual void Initialize() = 0;

    virtual void Resize(int width, int height) = 0;
    virtual void Draw(sf::RenderWindow *renderWindow);
    virtual void Update(sf::Time timeElapsed);
    virtual void HandleEvent(sf::Event event);

	void OpenWindow(const std::string &id, const network::protocol::WindowData &data);
	void UpdateWindow(const std::string &window, const network::protocol::UIData &data);

protected:
	UI *ui;
	std::list<uptr<Widget>> widgets;
	CustomWidget *curInputWidget;
	CustomWidget *underCursorWidget;

    virtual bool SetCurActiveWidget(Widget *);
};