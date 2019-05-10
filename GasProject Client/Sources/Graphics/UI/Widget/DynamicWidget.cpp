#include "DynamicWidget.h"

#include <fstream>

#include <imgui.h>
#include <imgui-SFML.h>

#include <Shared/JSON.hpp>

#include "LayoutRenderer.h"

using json = nlohmann::json;

DynamicWidget::DynamicWidget(const char *layout) {
	auto path = std::string("Resources/Layouts/") + layout;
	std::ifstream(path) >> this->layout;
}

void DynamicWidget::Update(sf::Time) {
	bool isOpen = true;
	LayoutRenderer(layout, &isOpen).Render();
	if (!isOpen)
		Close();
}
