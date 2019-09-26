#pragma once

#include <mutex>
#include <SFML/Graphics.hpp>

#include "Shared/Types.hpp"
#include "UIModules.hpp"

class UI {
private:
	sf::Vector2i size;
    sf::Font font;

    sf::Texture background;
    sf::Sprite background_sprite;

	uptr<UIModule> curUIModule;
	UIModule *newUIModule;

    mutable std::mutex mutex;

	uptr<Widget> console;

public:
    UI();
    UI(const UI &) = delete;
    UI &operator=(const UI &) = delete;
    virtual ~UI() = default;

    void Resize(int width, int height);
    void HandleEvent(sf::Event event);
    void Update(sf::Time timeElapsed);
    void Draw(sf::RenderWindow *render_window);

    void DrawMenuBackground(sf::RenderWindow *render_window);

	template<class ModuleType>
	void ChangeModule() { newUIModule = new ModuleType(this); }

    void Lock();
    void Unlock();

	const sf::Font &GetFont() const;
	UIModule *GetCurrentUIModule();
};