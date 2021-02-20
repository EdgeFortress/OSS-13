#pragma once

#include <cef_browser.h>

#include <Browser/Browser.h>

#include "BrowserHandlerImpl.h"

struct BrowserContent {
	std::string html;
	std::string resourcesPath;
};

class BrowserImpl : public Browser
{
public:
	BrowserImpl(CefRefPtr<CefBrowser> cefBrowser, CefRefPtr<BrowserHandlerImpl> handler);
	~BrowserImpl();

	// Browser
	void SetContent(const std::string &content) final;

	bool OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) final;
	bool OnMouseButtonReleased(sf::Mouse::Button button, uf::vec2i position) final;
	bool OnMouseMoved(uf::vec2i position) final;
	bool OnMouseLeft() final;
	bool OnMouseWheelScrolled(float delta, uf::vec2i position) final;
	bool OnKeyPressed(sf::Event::KeyEvent keyEvent) final;
	bool OnKeyReleased(sf::Event::KeyEvent keyEvent) final;
	bool OnTextEntered(uint32_t unicodeChar) final;

	std::future<void> Close();

private:
	uint32_t eventModifiers{};
	CefKeyEvent buffer;

	CefRefPtr<CefBrowser> cefBrowser;
	CefRefPtr<CefBrowserHost> cefBrowserHost;
	CefRefPtr<BrowserHandlerImpl> handler;

	std::shared_ptr<BrowserContent> content;
};
