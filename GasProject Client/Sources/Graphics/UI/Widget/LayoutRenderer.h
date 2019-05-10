#pragma once

#include <Shared/JSON.hpp>

using json = nlohmann::json;

class LayoutRenderer {
public:
	LayoutRenderer(json layout, bool *p_open);
	~LayoutRenderer();

	void Render();

private:
	json layout;

	bool isCollapsed;
};
