#include "imgui.h"

namespace ImGui {

IMGUI_API bool Button(const char* label, bool disabled, const ImVec2& size_arg = ImVec2(0, 0))
{
	if (disabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	bool result = ImGui::Button(label, size_arg);

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	return result;
}

}
