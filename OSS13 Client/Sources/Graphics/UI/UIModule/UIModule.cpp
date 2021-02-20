#include "UIModule.hpp"

#include <imgui.h>
#include <imgui-SFML.h>
#include <plog/Log.h>

#include <Graphics/UI/Widget/Widget.hpp>
#include <Graphics/UI/Widget/CustomWidget.h>
#include <Graphics/UI/Widget/DynamicWidget.h>

UIModule::UIModule(UI *ui) : ui(ui) {
	curInputWidget = nullptr;
	underCursorWidget = nullptr;
}

void UIModule::Draw(sf::RenderWindow *renderWindow) {
	for (auto &widget : widgets)
		widget->Draw(*renderWindow);
}

void UIModule::Update(sf::Time timeElapsed) {
	for (auto it = widgets.begin(); it != widgets.end();) {
		Widget *widget = it->get();
		if (widget->IsClosed())
			it = widgets.erase(it);
		else {
			widget->Update(timeElapsed);
			it++;
		}
	}
}

bool UIModule::HandleEvent(sf::Event event) {
	ImGuiIO& io = ImGui::GetIO();

	bool imguiCapture = false;

	switch (event.type) {
		case sf::Event::MouseWheelMoved:
		case sf::Event::MouseWheelScrolled:
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		case sf::Event::MouseMoved:
		case sf::Event::MouseEntered:
		case sf::Event::MouseLeft:
			if (io.WantCaptureMouse)
				imguiCapture = true;
			break;
		case sf::Event::TextEntered:
			if (io.WantTextInput)
				imguiCapture = true;
			break;
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			if (io.WantCaptureKeyboard)
				imguiCapture = true;
			break;
		default:
			break;
	}

	ImGui::SFML::ProcessEvent(event);
	if (imguiCapture)
		return true;

	switch (event.type) {
	case sf::Event::MouseWheelScrolled:
	case sf::Event::MouseButtonPressed:
	case sf::Event::MouseButtonReleased:
	{
		for (auto iter = widgets.rbegin(); iter != widgets.rend(); iter++) {
			if (auto customWidget = dynamic_cast<CustomWidget *>(iter->get())) {
				if (customWidget->HandleEvent(event)) {
					if (event.mouseButton.button == sf::Mouse::Left && customWidget->SetActive(true))
						if (customWidget != curInputWidget) {
							curInputWidget->SetActive(false);
							curInputWidget = customWidget;
						}
					return true;
				}
			}
		}
		break;
	}
	case sf::Event::MouseMoved: {
		for (auto iter = widgets.rbegin(); iter != widgets.rend(); iter++) {
			auto &widget = *iter;
			if (widget->HandleEvent(event)) {
				if (auto customWidget = dynamic_cast<CustomWidget *>(widget.get())) {
					if (underCursorWidget && underCursorWidget != customWidget) {
						sf::Event event;
						event.type = sf::Event::MouseLeft;
						underCursorWidget->HandleEvent(event); 
					}
					underCursorWidget = customWidget;
				}
				return true;
			}
		}
		if (underCursorWidget) {
			sf::Event event;
			event.type = sf::Event::MouseLeft;
			underCursorWidget->HandleEvent(event);
			underCursorWidget = nullptr;
		}
		break;
	}
	case sf::Event::MouseLeft: {
		if (underCursorWidget)
			underCursorWidget->HandleEvent(event);
		return true;
	}
	default:
		break;
	}
	if (curInputWidget)
		return curInputWidget->HandleEvent(event);
	return false;
}

bool UIModule::SetCurActiveWidget(Widget *newInputWidget) {
	if (auto customWidget = dynamic_cast<CustomWidget *>(newInputWidget)) {
		if (customWidget->SetActive(true)) {
			curInputWidget->SetActive(false);
			curInputWidget = customWidget;
			return true;
		}
	}
    return false;
}

void UIModule::OpenWindow(const std::string &id, const network::protocol::WindowData &data) {
	try {
		auto dynamicWidget = std::make_unique<DynamicWidget>(id);

		for (auto &field : data.fields) {
			dynamicWidget->SetData(*field);
		}

		widgets.push_back(std::move(dynamicWidget));
	} catch (const std::exception &e) {
		LOGE << "DynamicWindow with id \"" << id << "\" cannot be open!\n"
			 << "\tException: " << e.what();
	}
}

void UIModule::UpdateWindow(const std::string &window, const network::protocol::UIData &data) {
	for (auto &widget : widgets) {
		if (auto *dw = dynamic_cast<DynamicWidget *>(widget.get())) {
			if (dw->Id() == window)
				dw->SetData(data);
		}
	}
}
