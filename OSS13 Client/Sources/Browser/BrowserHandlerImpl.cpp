#include "BrowserHandlerImpl.h"

#include <SFML/OpenGL.hpp>

#include <cef_app.h>
#include <cef_parser.h>
#include <wrapper/cef_helpers.h>
#include <wrapper/cef_closure_task.h>

namespace {

// Returns a data: URI with the specified contents.
std::string GetDataURI(const std::string& data, const std::string& mime_type) {
	return "data:" + mime_type + ";base64," +
		CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
		.ToString();
}

void FillTextureWithBGRAPixels(const void* buffer, sf::Texture &bufferTexture)
{
	GLint textureBinding;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);
	sf::Texture::bind(&bufferTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bufferTexture.getSize().x, bufferTexture.getSize().y, GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, textureBinding);
}

} // namespace

BrowserHandlerImpl::BrowserHandlerImpl(BrowserUI &ui) :
	resourceManager(new CefResourceManager()),
	ui(ui),
	renderTexture(ui.GetRenderTexture())
{ }

CefRefPtr<CefDisplayHandler> BrowserHandlerImpl::GetDisplayHandler() { return this; }
CefRefPtr<CefLifeSpanHandler> BrowserHandlerImpl::GetLifeSpanHandler() { return this; }
CefRefPtr<CefLoadHandler> BrowserHandlerImpl::GetLoadHandler() { return this; }
CefRefPtr<CefRenderHandler> BrowserHandlerImpl::GetRenderHandler() { return this; }
CefRefPtr<CefRequestHandler> BrowserHandlerImpl::GetRequestHandler() { return this; }

CefRefPtr<CefResourceRequestHandler> BrowserHandlerImpl::GetResourceRequestHandler(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	bool is_navigation,
	bool is_download,
	const CefString& request_initiator,
	bool& disable_default_handling)
{
	return this;
}

void BrowserHandlerImpl::OnTitleChange(CefRefPtr<CefBrowser> browser,
								   const CefString& title) {
	CEF_REQUIRE_UI_THREAD();
}

void BrowserHandlerImpl::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	cefBrowser = browser;
	futureBrowserClose = promiseBrowserClose.get_future();
}

bool BrowserHandlerImpl::DoClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	return false;
}

void BrowserHandlerImpl::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	// TODO: check browser == cefBrowser
	cefBrowser = nullptr;
	promiseBrowserClose.set_value();
}

CefRefPtr<CefAccessibilityHandler> BrowserHandlerImpl::GetAccessibilityHandler() { return nullptr; }

bool BrowserHandlerImpl::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) { return false; }

void BrowserHandlerImpl::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
	rect.width = renderTexture.getSize().x;
	rect.height = renderTexture.getSize().y;
	bufferTexture.create(renderTexture.getSize().x, renderTexture.getSize().y);
	bufferSprite.setTexture(bufferTexture, true);
}

bool BrowserHandlerImpl::GetScreenPoint(
	CefRefPtr<CefBrowser> browser,
	int viewX,
	int viewY,
	int& screenX,
	int& screenY)
{
	auto screenPoint = ui.CastViewPointToScreenPoint({ viewX, viewY });
	screenX = screenPoint.x;
	screenY = screenPoint.y;
	return true;
}

bool BrowserHandlerImpl::GetScreenInfo(
	CefRefPtr<CefBrowser> browser,
	CefScreenInfo& screen_info)
{
	return false;
}

void BrowserHandlerImpl::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) { }

void BrowserHandlerImpl::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) { }

void BrowserHandlerImpl::OnPaint(
	CefRefPtr<CefBrowser> browser,
	PaintElementType type,
	const RectList& dirtyRects,
	const void* buffer,
	int width,
	int height)
{
	FillTextureWithBGRAPixels(buffer, bufferTexture);
	renderTexture.clear(sf::Color::White);
	renderTexture.draw(bufferSprite);
	renderTexture.display();
}

void BrowserHandlerImpl::OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
	PaintElementType type,
	const RectList& dirtyRects,
	void* shared_handle) {
}

bool BrowserHandlerImpl::StartDragging(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDragData> drag_data,
	DragOperationsMask allowed_ops,
	int x,
	int y) {
	return true;
}

void BrowserHandlerImpl::UpdateDragCursor(CefRefPtr<CefBrowser> browser,
	DragOperation operation) {}

void BrowserHandlerImpl::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,
	double x,
	double y) {}

void BrowserHandlerImpl::OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser,
	const CefRange& selected_range,
	const RectList& character_bounds) {}

void BrowserHandlerImpl::OnTextSelectionChanged(CefRefPtr<CefBrowser> browser,
	const CefString& selected_text,
	const CefRange& selected_range) {}

void BrowserHandlerImpl::OnVirtualKeyboardRequested(CefRefPtr<CefBrowser> browser,
	TextInputMode input_mode) {}

void BrowserHandlerImpl::OnLoadError(CefRefPtr<CefBrowser> browser,
								 CefRefPtr<CefFrame> frame,
								 ErrorCode errorCode,
								 const CefString& errorText,
								 const CefString& failedUrl) {
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED)
		return;

	  // Display a load error message using a data: URI.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL "
		<< std::string(failedUrl) << " with error " << std::string(errorText)
		<< " (" << errorCode << ").</h2></body></html>";

	frame->LoadURL(GetDataURI(ss.str(), "text/html"));
}

cef_return_value_t BrowserHandlerImpl::OnBeforeResourceLoad(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefRequestCallback> callback)
{
	CEF_REQUIRE_IO_THREAD();
	return resourceManager->OnBeforeResourceLoad(browser, frame, request, callback);
}

CefRefPtr<CefResourceHandler> BrowserHandlerImpl::GetResourceHandler(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request)
{
	CEF_REQUIRE_IO_THREAD();
	return resourceManager->GetResourceHandler(browser, frame, request);
}

std::future<void> BrowserHandlerImpl::GetCloseFuture() {
	return std::move(futureBrowserClose);
}

CefRefPtr<CefResourceManager> BrowserHandlerImpl::GetResourceManager() {
	return resourceManager;
}
