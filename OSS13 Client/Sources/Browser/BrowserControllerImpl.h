#pragma once

#include <mutex>

#include <cef_app.h>

#include <Browser/BrowserController.h>

#include "BrowserImpl.h"
#include "BrowserControllerHandlers.h"

class BrowserControllerImpl : 
	public BrowserController
{
public:
	BrowserControllerImpl();
	~BrowserControllerImpl();

	void Initialize(const std::filesystem::path& executablePath) final;
	void Update() final;
	void Shutdown() final;
	bool CanBeReleased() final;

	Browser &CreateBrowser(BrowserUI &ui) final;
	void CloseBrowser(Browser *browser) final;

private:
	CefRefPtr<BrowserControllerHandlers> handlers;
	std::vector<std::unique_ptr<BrowserImpl>> browsers;
	std::vector<std::future<void>> browserCloseFutures;
	std::thread browsersThread;
};
