#pragma once

#include <filesystem>

#include <Shared/IFaces/IFace.h>

#include <Browser/Browser.h>
#include <Browser/BrowserUI.h>

struct BrowserController : public IFace
{
	virtual void Initialize(const std::filesystem::path &executablePath) = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;
	virtual bool CanBeReleased() = 0;

	virtual Browser &CreateBrowser(BrowserUI &ui) = 0;
	virtual void CloseBrowser(Browser* browser) = 0;
};

std::unique_ptr<BrowserController> CreateBrowserConroller();
