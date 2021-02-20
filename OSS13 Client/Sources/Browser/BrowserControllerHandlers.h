#pragma once

#include <cef_app.h>
#include <cef_base.h>

class BrowserControllerImpl;

class BrowserControllerHandlers :
    public CefApp,
    public CefBrowserProcessHandler
{
public:
    BrowserControllerHandlers(BrowserControllerImpl &browserController);

    // CefApp
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() final;

    // CefBrowserProcessHandler
    void OnContextInitialized() final;

private:
    BrowserControllerImpl& browserController;
    IMPLEMENT_REFCOUNTING(BrowserControllerHandlers);
};
