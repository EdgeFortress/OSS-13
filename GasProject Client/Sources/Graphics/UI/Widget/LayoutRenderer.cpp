#include "LayoutRenderer.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <Shared/Geometry/Vec2.hpp>

namespace uf {

template<class T>
void from_json(const json& j, uf::vec2<T>& p) {
	j[0].get_to(p.x);
	j[1].get_to(p.y);
}

}

void RenderText(json layout);

LayoutRenderer::LayoutRenderer(json layout, bool *p_open) :
	layout(layout),
	isCollapsed(false)
{
	auto id = layout["id"].get<std::string>();

	json::iterator it;

	it = layout.find("position");
	if (it != layout.end())
		ImGui::SetNextWindowPos(it->get<uf::vec2f>(), ImGuiCond_Once);

	it = layout.find("size");
	if (it != layout.end())
		ImGui::SetNextWindowSize(it->get<uf::vec2f>(), ImGuiCond_Once);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings;


	if (!ImGui::Begin(id.c_str(), p_open, flags))
		isCollapsed = true;
}

LayoutRenderer::~LayoutRenderer() {
	ImGui::End();
}

void LayoutRenderer::Render() {
	if (isCollapsed)
		return;

	auto it = layout.find("content");
	if (it == layout.end())
		return;

	for (auto &widgetLayout : *it) {
		RenderText(widgetLayout);
	}
}

void RenderText(json layout) {
	auto id = layout["fmt"].get<std::string>();

	ImGui::TextWrapped(id.c_str());
}
