#include "DynamicWidget.h"

#include <fstream>

#include <imgui.h>
#include <imgui-SFML.h>

#include <Shared/JSON.hpp>
#include <Shared/Geometry/Vec2.hpp>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>

#include <Network.hpp>

using json = nlohmann::json;
using namespace network::protocol;

namespace uf
{

template<class T>
void from_json(const json& j, uf::vec2<T>& p) {
	j[0].get_to(p.x);
	j[1].get_to(p.y);
}

}


DynamicWidget::DynamicWidget(const std::string &id) :
	id(id)
{
	auto path = std::string("Resources/Layouts/") + id + ".json";
	std::ifstream(path) >> layout;
}

void DynamicWidget::Update(sf::Time) {
	render();
	sendUpdates();
}

void DynamicWidget::render() {
	json::iterator it;

	std::string title = id;
	it = layout.find("title");
	if (it != layout.end())
		title = it->get<std::string>();

	it = layout.find("position");
	if (it != layout.end())
		ImGui::SetNextWindowPos(it->get<uf::vec2f>(), ImGuiCond_Once);

	it = layout.find("size");
	if (it != layout.end())
		ImGui::SetNextWindowSize(it->get<uf::vec2f>(), ImGuiCond_Once);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize;

	bool isOpen = true;

	if (!ImGui::Begin(title.c_str(), &isOpen, flags)) {
		ImGui::End();
		return;
	}

	it = layout.find("content");
	if (it != layout.end())
		for (auto &widgetLayout : *it) {
			static const std::map<std::string, std::function<void(DynamicWidget *, json &)>> handlers {
				{ "TestWidget", &DynamicWidget::renderTest },
				{ "TextWrapped", std::function<void(DynamicWidget *, json &)>(&DynamicWidget::renderText) },
				{ "Button", std::function<void(DynamicWidget *, json &)>(&DynamicWidget::renderButton) },
				{ "RadioButton", std::function<void(DynamicWidget *, json &)>(&DynamicWidget::renderRadio) }
			};

			const std::string type = widgetLayout["type"];
			handlers.at(type)(this, widgetLayout);
		}

	ImGui::End();

	if (!isOpen) {
		triggers.push(ImGuiTrigger{std::string("Close")});
		Close();
	}
}

void DynamicWidget::sendUpdates() {
	for (auto &pair : handles) {
		if (pair.second.IsChanged()) {
			auto &&data = pair.second.GetInputData();
			data->window = id;
			data->handle = pair.first;
			auto *p = new client::UIInputCommand();
			p->handle = pair.first;
			p->data = std::move(data);
			Connection::commandQueue.Push(p);
		}
	}

	while (!triggers.empty()) {
		auto trigger = std::move(triggers.front()); triggers.pop();
		auto *p = new client::UITriggerCommand();
		p->trigger = trigger.trigger;
		p->window = id;
		Connection::commandQueue.Push(p);
	}
}

void DynamicWidget::renderTest(json &layout) {
	
}

void DynamicWidget::renderText(json &layout) {
	auto fmt = layout["fmt"].get<std::string>();

	ImGui::TextWrapped(fmt.c_str());
}

void DynamicWidget::renderButton(json &layout) {
	auto label = layout["label"].get<std::string>();
	auto trigger = layout["trigger"].get<std::string>();

	uf::vec2f size;
	auto it = layout.find("size");
	if (it != layout.end()) size = it->get<uf::vec2f>();

	if (ImGui::Button(label.c_str(), size)) {
		triggers.push(ImGuiTrigger{ trigger });
	};
}

void DynamicWidget::renderRadio(json &layout) {
	auto label = layout["label"].get<std::string>();
	auto handle = layout["handle"].get<std::string>();
	auto type = layout["radio_button_type"].get<std::string>();
	auto value = layout["value"].get<int>();

	auto &handleRef = handles[handle];

	auto buffer = handleRef.GetData<int>();
	if (type == "int") {
		ImGui::RadioButton(label.c_str(), &buffer, value);
	}
	handleRef.SetData<int>(buffer);
}
