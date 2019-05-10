#pragma once

#include <Graphics/UI/Widget/ImGuiWidget.h>

#include <Shared/JSON.hpp>

using json = nlohmann::json;

class DynamicWidget : public ImGuiWidget {
public:
	DynamicWidget(const char *layout);

// ImGuiWidget
	void Update(sf::Time timeElapsed);

private:
	json layout;
};
