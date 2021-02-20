#pragma once

#include <list>
#include <future>

#include <cef_client.h>
#include <cef_browser.h>
#include <cef_display_handler.h>
#include <cef_life_span_handler.h>
#include <wrapper/cef_resource_manager.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <Browser/Browser.h>
#include <Browser/BrowserUI.h>

class BrowserHandlerImpl :
	public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefRenderHandler,
	public CefLoadHandler,
	public CefRequestHandler,
	public CefResourceRequestHandler
{
public:
	explicit BrowserHandlerImpl(BrowserUI &ui);

	// CefClient
	CefRefPtr<CefDisplayHandler> GetDisplayHandler() final;
	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() final;
	CefRefPtr<CefLoadHandler> GetLoadHandler() final;
	CefRefPtr<CefRenderHandler> GetRenderHandler() final;
	CefRefPtr<CefRequestHandler> GetRequestHandler() final;

	// CefRequestHandler
	CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		bool is_navigation,
		bool is_download,
		const CefString& request_initiator,
		bool& disable_default_handling) final;

	// CefDisplayHandler
	void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) final;

	// CefLifeSpanHandler
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) final;
	bool DoClose(CefRefPtr<CefBrowser> browser) final;
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) final;

	// CefRenderHandler
	CefRefPtr<CefAccessibilityHandler> GetAccessibilityHandler() final;
	bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) final;
	void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) final;
	bool GetScreenPoint(
		CefRefPtr<CefBrowser> browser,
		int viewX,
		int viewY,
		int& screenX,
		int& screenY) final;
	bool GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info) final;
	void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) final;
	void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) final;
	void OnPaint(
		CefRefPtr<CefBrowser> browser,
		PaintElementType type,
		const RectList& dirtyRects,
		const void* buffer,
		int width,
		int height) final;
	void OnAcceleratedPaint(
		CefRefPtr<CefBrowser> browser,
		PaintElementType type,
		const RectList& dirtyRects,
		void* shared_handle) final;
	bool StartDragging(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDragData> drag_data,
		DragOperationsMask allowed_ops,
		int x,
		int y) final;
	void UpdateDragCursor(CefRefPtr<CefBrowser> browser, DragOperation operation) final;
	void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y) final;
	void OnImeCompositionRangeChanged(
		CefRefPtr<CefBrowser> browser,
		const CefRange& selected_range,
		const RectList& character_bounds) final;
	void OnTextSelectionChanged(
		CefRefPtr<CefBrowser> browser,
		const CefString& selected_text,
		const CefRange& selected_range) final;
	void OnVirtualKeyboardRequested(
		CefRefPtr<CefBrowser> browser,
		TextInputMode input_mode) final;

	// CefLoadHandler
	void OnLoadError(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) final;

	// CefResourceRequestHandler
	cef_return_value_t OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefRequestCallback> callback) final;
	CefRefPtr<CefResourceHandler> GetResourceHandler(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request) final;

	// BrowserHandlerImpl
	std::future<void> GetCloseFuture();
	CefRefPtr<CefResourceManager> GetResourceManager();

private:
	CefRefPtr<CefBrowser> cefBrowser;
	CefRefPtr<CefResourceManager> resourceManager;

	BrowserUI &ui;
	sf::RenderTexture &renderTexture;

	sf::Sprite bufferSprite;
	sf::Texture bufferTexture;

	std::future<void> futureBrowserClose;
	std::promise<void> promiseBrowserClose;

	IMPLEMENT_REFCOUNTING(BrowserHandlerImpl);
	DISALLOW_COPY_AND_ASSIGN(BrowserHandlerImpl);
};
