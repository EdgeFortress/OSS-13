#pragma once

#include <list>

#include "Shared/Types.hpp"

class UI;
class Widget;

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

protected:
	UI *ui;
	std::list<uptr<Widget>> widgets;
	Widget *curInputWidget;
};