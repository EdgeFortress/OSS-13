#include "BrowserWidget.h"

#include <Client.hpp>
#include <Graphics/Window.hpp>

BrowserWidget::BrowserWidget(uf::vec2i size, BrowserController &browserController) :
	CustomWidget(size),
	browserController(browserController)
{
	canBeActive = true;
	browser = &browserController.CreateBrowser(*this);
}

BrowserWidget::~BrowserWidget()
{
	browserController.CloseBrowser(browser);
}

bool BrowserWidget::OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) {
	if (!(position >= GetAbsolutePosition() && position < GetAbsolutePosition() + GetSize()))
		return false;
	return browser->OnMouseButtonPressed(button, position - GetPosition());
}

bool BrowserWidget::OnMouseButtonReleased(sf::Mouse::Button button, uf::vec2i position) {
	if (!(position >= GetAbsolutePosition() && position < GetAbsolutePosition() + GetSize()))
		return false;
	return browser->OnMouseButtonReleased(button, position - GetPosition());
}

bool BrowserWidget::OnMouseMoved(uf::vec2i position) {
	if (!(position >= GetAbsolutePosition() && position < GetAbsolutePosition() + GetSize()))
		return false;
	return browser->OnMouseMoved(position - GetPosition());
}

bool BrowserWidget::OnMouseLeft() {
	return browser->OnMouseLeft();
}

bool BrowserWidget::OnMouseWheelScrolled(float delta, uf::vec2i position) {
	if (!(position >= GetAbsolutePosition() && position < GetAbsolutePosition() + GetSize()))
		return false;
	return browser->OnMouseWheelScrolled(delta, position - GetPosition());
}

bool BrowserWidget::OnKeyPressed(sf::Event::KeyEvent keyEvent) {
	return browser->OnKeyPressed(keyEvent);
}

bool BrowserWidget::OnKeyReleased(sf::Event::KeyEvent keyEvent) {
	return browser->OnKeyReleased(keyEvent);
}

bool BrowserWidget::OnTextEntered(uint32_t unicodeChar) {
	return browser->OnTextEntered(unicodeChar);
}

uf::vec2i BrowserWidget::CastViewPointToScreenPoint(uf::vec2i viewPoint) {
	auto c = CC::Get();
	auto w = c->GetWindow();

	auto windowPosition = w->GetPosition();
	return windowPosition + GetAbsolutePosition() + viewPoint;
}

sf::WindowHandle BrowserWidget::GetWindowHandle() {
	return CC::Get()->GetWindow()->GetSystemHandle();
}

sf::RenderTexture &BrowserWidget::GetRenderTexture() {
	return buffer;
}

void BrowserWidget::SetContent(const std::string& content) {
	browser->SetContent(content);
}
