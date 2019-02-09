#include "UIModule.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

#include "Graphics/UI/Widget/Widget.hpp"

UIModule::UIModule(UI *ui) : ui(ui) {
	curInputWidget = nullptr;
	underCursorWidget = nullptr;
}

void UIModule::Draw(sf::RenderWindow *renderWindow) {
	for (auto &widget : widgets)
		widget->Draw(*renderWindow);
}

void UIModule::Update(sf::Time timeElapsed) {
	ImGui::ShowTestWindow();
	for (auto &widget : widgets)
		widget->Update(timeElapsed);
}

void UIModule::HandleEvent(sf::Event event) {
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
	}

	ImGui::SFML::ProcessEvent(event);
	if (imguiCapture)
		return;

	switch (event.type) {
	case sf::Event::MouseButtonPressed: {
        if (event.mouseButton.button == sf::Mouse::Left) {
            for (auto &widget : widgets)
                if (widget->HandleEvent(event)) {
                    if (widget->SetActive(true))
                        if (widget.get() != curInputWidget) {
                            curInputWidget->SetActive(false);
                            curInputWidget = widget.get();
                        }
                    return;
                }
        }
		break;
	}
	case sf::Event::MouseMoved: {
		for (auto &widget : widgets) {
			if (widget->HandleEvent(event)) {
				if (underCursorWidget && underCursorWidget != widget.get()) { 
					sf::Event event;
					event.type = sf::Event::MouseLeft;
					underCursorWidget->HandleEvent(event); 
				}
				underCursorWidget = widget.get();
				return;
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
		return;
	}
	default:
        break;
	}
    if (curInputWidget)
        curInputWidget->HandleEvent(event);
}

bool UIModule::SetCurActiveWidget(Widget *newInputWidget) {
    if (newInputWidget->SetActive(true)) {
        curInputWidget->SetActive(false);
        curInputWidget = newInputWidget;
        return true;
    }
    return false;
}