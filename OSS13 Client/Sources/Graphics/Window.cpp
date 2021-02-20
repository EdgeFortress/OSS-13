#include "Window.hpp"

#include <fstream>
#include <imgui.h>
#include <imgui-SFML.h>

#include <Client.hpp>
#include <ResourceManager.hpp>
#include <Graphics/TileGrid/TileGrid.hpp>
#include <Graphics/UI/UI.hpp>

#include <Shared/IFaces/IConfig.h>
#include <Shared/JSON.hpp>
#include <Shared/OS.hpp>

Window::Window() :
	ui(new UI),
	browserController(CreateBrowserConroller())
{ }

void setImGuiStyle(ImGuiStyle& style);

uf::vec2i GetResolution(const IConfig *config) {
	EXPECT(config);
	return config->GetVec2i("Graphics.Resolution");
}

void Window::Initialize(const std::filesystem::path& executablePath) {
    resolution = GetResolution(CC::Get()->RM.Config());
    window.reset(new sf::RenderWindow(sf::VideoMode(resolution.x, resolution.y), "Open Space Station 13"));

    ui->ChangeModule<AuthUI>();
    resize(window->getSize().x, window->getSize().y);

	browserController->Initialize(executablePath);
	ui->Initialize();

	ImGui::SFML::Init(*window);
	setImGuiStyle(ImGui::GetStyle());
	ImGui::GetIO().Fonts->Clear();
	ImGui::GetIO().Fonts->AddFontFromFileTTF("Arialuni.ttf", 16, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();
}

void Window::Update() {
	browserController->Update();

	if (isClosing) {
		if (browserController->CanBeReleased()) {
			browserController->Shutdown();
			window->close();
		}
		return;
	}

	sf::Event event;
	while (window->pollEvent(event)) {
		// TODO: find the better way to obtain window's view coords (respecting window's titlebar and borders)
		if (event.type == sf::Event::MouseButtonPressed) {
			auto &mouseEvent = reinterpret_cast<sf::Event::MouseButtonEvent &>(event);
			// (screen related coords) - (window related coords)
			viewPosition = uf::vec2i(sf::Mouse::getPosition()) - uf::vec2i(event.mouseButton.x, event.mouseButton.y);
		}

		ui->HandleEvent(event);
		if (event.type == sf::Event::Resized)
			resize(event.size.width, event.size.height);
		if (event.type == sf::Event::Closed)
		{
			isClosing = true;
			onClose();
			return;
		}
	}

	window->clear(sf::Color::Black);
	ImGui::SFML::Update(*window, lastFrameTime);
	ui->Update(lastFrameTime);
	ui->Draw(window.get());

	ImGui::SFML::Render(*window);
	window->display();

	fps_sleep();
}

bool Window::isOpen() const { return window->isOpen(); }
int Window::GetWidth() const { return resolution.x; }
int Window::GetHeight() const { return resolution.y; }
uf::vec2i Window::GetPosition() const { return viewPosition; }
BrowserController* Window::GetBrowserController() const { return browserController.get(); }
UI *Window::GetUI() const { return ui.get(); }
sf::WindowHandle Window::GetSystemHandle() const { return window->getSystemHandle(); }

void Window::fps_sleep() {
	sf::Time timeElapsed = frame_clock.getElapsedTime();

	if (timeElapsed.asMicroseconds() < 1000 * 1000 / req_FPS)
		sf::sleep(sf::microseconds(1000 * 1000 / req_FPS) - timeElapsed);

	lastFrameTime = frame_clock.restart();
}

void Window::resize(const int newWidth, const int newHeight) {
	resolution = {newWidth, newHeight};
	const sf::FloatRect visibleArea(0, 0, float(newWidth), float(newHeight));
	window->setView(sf::View(visibleArea));
	ui->Resize(newWidth, newHeight);
}

void setImGuiStyle(ImGuiStyle& style) {
	style.WindowRounding = 0;
	style.FrameRounding = 0;

	style.Colors[ImGuiCol_Text] = { 0.73333335f, 0.73333335f, 0.73333335f, 1.00f };
	style.Colors[ImGuiCol_TextDisabled] = { 0.34509805f, 0.34509805f, 0.34509805f, 1.00f };
	style.Colors[ImGuiCol_WindowBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.94f };
	style.Colors[ImGuiCol_ChildBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.00f };
	style.Colors[ImGuiCol_PopupBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.94f };
	style.Colors[ImGuiCol_Border] = { 0.33333334f, 0.33333334f, 0.33333334f, 0.50f };
	style.Colors[ImGuiCol_BorderShadow] = { 0.15686275f, 0.15686275f, 0.15686275f, 0.00f };
	style.Colors[ImGuiCol_FrameBg] = { 0.16862746f, 0.16862746f, 0.16862746f, 0.54f };
	style.Colors[ImGuiCol_FrameBgHovered] = { 0.453125f, 0.67578125f, 0.99609375f, 0.67f };
	style.Colors[ImGuiCol_FrameBgActive] = { 0.47058827f, 0.47058827f, 0.47058827f, 0.67f };
	style.Colors[ImGuiCol_TitleBg] = { 0.2f, 0.2f, 0.2f, 1.00f };
	style.Colors[ImGuiCol_TitleBgCollapsed] = { 0.16f, 0.29f, 0.48f, 1.00f };
	style.Colors[ImGuiCol_TitleBgActive] = { 0.1f, 0.1f, 0.1f, 1.00f };
	style.Colors[ImGuiCol_MenuBarBg] = { 0.27058825f, 0.28627452f, 0.2901961f, 0.80f };
	style.Colors[ImGuiCol_ScrollbarBg] = { 0.27058825f, 0.28627452f, 0.2901961f, 0.60f };
	style.Colors[ImGuiCol_ScrollbarGrab] = { 0.21960786f, 0.30980393f, 0.41960788f, 0.51f };
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = { 0.21960786f, 0.30980393f, 0.41960788f, 1.00f };
	style.Colors[ImGuiCol_ScrollbarGrabActive] = { 0.13725491f, 0.19215688f, 0.2627451f, 0.91f };
	style.Colors[ImGuiCol_CheckMark] = { 0.90f, 0.90f, 0.90f, 0.83f };
	style.Colors[ImGuiCol_SliderGrab] = { 0.70f, 0.70f, 0.70f, 0.62f };
	style.Colors[ImGuiCol_SliderGrabActive] = { 0.30f, 0.30f, 0.30f, 0.84f };
	style.Colors[ImGuiCol_Button] = { 0.33333334f, 0.3529412f, 0.36078432f, 0.49f };
	style.Colors[ImGuiCol_ButtonHovered] = { 0.21960786f, 0.30980393f, 0.41960788f, 1.00f };
	style.Colors[ImGuiCol_ButtonActive] = { 0.13725491f, 0.19215688f, 0.2627451f, 1.00f };
	style.Colors[ImGuiCol_Header] = { 0.33333334f, 0.3529412f, 0.36078432f, 0.53f };
	style.Colors[ImGuiCol_HeaderHovered] = { 0.453125f, 0.67578125f, 0.99609375f, 0.67f };
	style.Colors[ImGuiCol_HeaderActive] = { 0.47058827f, 0.47058827f, 0.47058827f, 0.67f };
	style.Colors[ImGuiCol_Separator] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
	style.Colors[ImGuiCol_SeparatorHovered] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
	style.Colors[ImGuiCol_SeparatorActive] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
	style.Colors[ImGuiCol_ResizeGrip] = { 0.2f, 0.2f, 0.2f, 0.5f };
	style.Colors[ImGuiCol_ResizeGripHovered] = { 0.16f, 0.29f, 0.48f, 0.5f };
	style.Colors[ImGuiCol_ResizeGripActive] = { 0.1f, 0.1f, 0.1f, 0.5f };
	style.Colors[ImGuiCol_PlotLines] = { 0.61f, 0.61f, 0.61f, 1.00f };
	style.Colors[ImGuiCol_PlotLinesHovered] = { 1.00f, 0.43f, 0.35f, 1.00f };
	style.Colors[ImGuiCol_PlotHistogram] = { 0.90f, 0.70f, 0.00f, 1.00f };
	style.Colors[ImGuiCol_PlotHistogramHovered] = { 1.00f, 0.60f, 0.00f, 1.00f };
	style.Colors[ImGuiCol_TextSelectedBg] = { 0.18431373f, 0.39607847f, 0.79215693f, 0.90f };
}

void Window::onClose() {
	ui.reset();
}
