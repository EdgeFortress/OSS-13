#include "BrowserControllerImpl.h"

#include <algorithm>
#include <filesystem>

#include <cef_app.h>

#include "BrowserHandlerImpl.h"

BrowserControllerImpl::BrowserControllerImpl()
{ }

BrowserControllerImpl::~BrowserControllerImpl()
{ }

void BrowserControllerImpl::Initialize(const std::filesystem::path &executablePath)
{
    CefMainArgs main_args;

    CefSettings settings;
    settings.command_line_args_disabled = true;
    settings.windowless_rendering_enabled = true;
    settings.external_message_pump = true;
    settings.log_severity = LOGSEVERITY_DEBUG;
    CefString(&settings.cache_path).FromString((std::filesystem::temp_directory_path() / "OSS13 Cache").string());
    CefString(&settings.browser_subprocess_path).FromString((executablePath / "oss13.browser.exe").string());
    CefString(&settings.resources_dir_path).FromString((std::filesystem::current_path() / "Resources" / "Browser").string());
    CefString(&settings.locales_dir_path).FromString((std::filesystem::current_path() / "Resources" / "Browser" / "locales").string());

    handlers = new BrowserControllerHandlers(*this);

    CefInitialize(main_args, settings, handlers, nullptr);
}

void BrowserControllerImpl::Update()
{
    CefDoMessageLoopWork();
}

void BrowserControllerImpl::Shutdown()
{
    for (auto &browser: browsers)
        browserCloseFutures.push_back(browser->Close());
    browsers.clear();
    handlers = nullptr;

    // TODO: check if browserCloseFutures is empty

    CefShutdown();
}

bool BrowserControllerImpl::CanBeReleased()
{
    // TODO: check if browsers is empty

    browserCloseFutures.erase(
        std::remove_if(
            browserCloseFutures.begin(),
            browserCloseFutures.end(),
            [](auto &future){ return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }),
        browserCloseFutures.end());

    return browserCloseFutures.empty();
}

Browser &BrowserControllerImpl::CreateBrowser(BrowserUI &ui)
{
    // SimpleHandler implements browser-level callbacks.
    CefRefPtr<BrowserHandlerImpl> handler(new BrowserHandlerImpl(ui));

    // Specify CEF browser settings here.
    CefBrowserSettings browser_settings;

    CefWindowInfo window_info;
    window_info.SetAsWindowless(ui.GetWindowHandle());
    window_info.shared_texture_enabled = false;

    auto cefBrowser = CefBrowserHost::CreateBrowserSync(window_info, handler, "", browser_settings, nullptr, nullptr);
    browsers.push_back(std::make_unique<BrowserImpl>(cefBrowser, handler));
    return *browsers.back().get();
}

void BrowserControllerImpl::CloseBrowser(Browser* browser)
{
    for (auto iter = browsers.begin(); iter != browsers.end(); iter++)
        if (iter->get() == browser) {
            browserCloseFutures.push_back(iter->get()->Close());
            browsers.erase(iter);
            break;
        }
}

std::unique_ptr<BrowserController> CreateBrowserConroller()
{
    return std::make_unique<BrowserControllerImpl>();
}
