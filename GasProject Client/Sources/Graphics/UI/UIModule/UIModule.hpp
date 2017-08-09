#pragma once

class UI;

namespace sf {
	class RenderWindow;
	class Time;
	class Event;
}

class UIModule {
protected:
	UI *ui;

public:
	UIModule(UI *ui);
	UIModule(const UIModule &) = delete;
	UIModule &operator=(const UIModule &) = delete;
	virtual ~UIModule() = default;

	virtual void Resize(int width, int height) = 0;
	virtual void Draw(sf::RenderWindow *renderWindow) = 0;
	virtual void Update(sf::Time timeElapsed) = 0;
	virtual void HandleEvent(sf::Event event) = 0;

	virtual void Hide() = 0;
	virtual void Show() = 0;
};