#include "BrowserControllerHandlers.h"

#include "BrowserControllerImpl.h"

BrowserControllerHandlers::BrowserControllerHandlers(BrowserControllerImpl& browserController) :
    browserController(browserController)
{ }

CefRefPtr<CefBrowserProcessHandler> BrowserControllerHandlers::GetBrowserProcessHandler() {
    return this;
}

void BrowserControllerHandlers::OnContextInitialized()
{ }
