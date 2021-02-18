#pragma once

#include <Browser/BrowserController.h>
#include <Browser/BrowserUI.h>
#include <Graphics/UI/Widget/CustomWidget.h>
#include <Graphics/UI/Widget/Container.hpp>

class BrowserWidget : public CustomWidget, public BrowserUI
{
public:
	BrowserWidget(uf::vec2i size, BrowserController &browserController);
	~BrowserWidget();

	// CustomWidget
	void Update(sf::Time timeElapsed) final { };

	bool OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) final;
	bool OnMouseButtonReleased(sf::Mouse::Button button, uf::vec2i position) final;
	bool OnMouseMoved(uf::vec2i position) final;
	bool OnMouseLeft() final;
	bool OnMouseWheelScrolled(float delta, uf::vec2i position) final;
	bool OnKeyPressed(sf::Event::KeyEvent keyEvent) final;
	bool OnKeyReleased(sf::Event::KeyEvent keyEvent) final;
	bool OnTextEntered(uint32_t unicodeChar) final;

	// BrowserUI
	uf::vec2i CastViewPointToScreenPoint(uf::vec2i viewPoint) final;

	sf::WindowHandle GetWindowHandle() final;
	sf::RenderTexture &GetRenderTexture() final;

	// BrowserWidget
	void SetContent(const std::string &content);

private:
	void draw() const final { };

private:
	BrowserController &browserController;
	Browser *browser;
};
