#include "SpawnWindow.h"

#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui_extended.h>

#include <Client.hpp>
#include <Network/Connection.h>
#include <Graphics/Sprite.hpp>

#include <Shared/Network/Protocol/ClientToServer/Commands.h>


void SpawnWindow::Update(sf::Time timeElapsed) {
	std::unique_lock<std::mutex> lock(guard);

	ImGui::SetNextWindowPos(ImVec2(60, 60), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);

	if (!ImGui::Begin("Object Spawner")) {
		ImGui::End();
		return;
	}

	drawHeader();
	drawBody();

	ImGui::End();
}

void SpawnWindow::UpdateTypes(std::vector<network::protocol::ObjectType> &&types) {
	std::unique_lock<std::mutex> lock(guard);

	this->types = std::forward<std::vector<network::protocol::ObjectType>>(types);
	noQueriesYet = false;
}

void searchTypesAndDropBuffer(std::string &searchBuffer) {
	auto command = std::make_unique<network::protocol::client::SpawnWindowSearchCommand>();
	std::swap(command->searchBuffer, searchBuffer);
	Connection::commandQueue.Push(command.release());
}

void drawHeaderInput(std::string &searchBuffer) {
	if (ImGui::InputTextWithHint("", "all object types", &searchBuffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
		searchTypesAndDropBuffer(searchBuffer);
	}
}

void drawHeaderSearchButton(std::string &searchBuffer) {
	ImGui::SameLine();
	if (ImGui::Button("Search")) {
		searchTypesAndDropBuffer(searchBuffer);
	}
}

void SpawnWindow::drawHeader() {
	drawHeaderInput(searchBuffer);
	drawHeaderSearchButton(searchBuffer);
}

void drawTypesListItemTooltip(const network::protocol::ObjectType &type) {
	if (type.sprite) {
		auto sprite = CC::Get()->RM.CreateSprite(type.sprite);
		ImGui::Image(sprite.GetSfmlSprite());
		ImGui::SameLine();
	}

	ImGui::BeginGroup();
	ImGui::Text(type.name.c_str());
	ImGui::Text(type.typeKey.c_str());
	ImGui::Text(type.description.c_str());
	ImGui::EndGroup();
}

void drawTypesListItem(const network::protocol::ObjectType &type, bool &selected) {
	ImGui::Selectable(type.typeKey.c_str(), &selected);
		
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		drawTypesListItemTooltip(type);
		ImGui::EndTooltip();
	}
}

void drawTypesListEmptySearchResultText() {
	ImGui::Text("No types were found!");
}

void drawTypesListContent(const std::vector<network::protocol::ObjectType> &types, size_t &selectedIndex) {
	size_t counter = 0;
	for (auto &type : types) {
		counter++;
		bool selected = (counter == selectedIndex);
		drawTypesListItem(type, selected);
		if (selected)
			selectedIndex = counter;
	}
}

void SpawnWindow::drawTypesList() {
	const float footerHeightToReserve = ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("types list", ImVec2(0, -footerHeightToReserve), true);

	if (!types.size() && !noQueriesYet)
		drawTypesListEmptySearchResultText();
	else
		drawTypesListContent(types, selectedIndex);

	ImGui::EndChild();
}

void spawnObject(const std::string &typeKey) {
	auto command = std::make_unique<network::protocol::client::SpawnWindowSpawnCommand>();
	command->typeKey = typeKey;
	Connection::commandQueue.Push(command.release());
}

void SpawnWindow::drawSpawnButton() {
	bool disabled = (selectedIndex == 0);
	if (ImGui::Button("Spawn", disabled)) {
		spawnObject(types[selectedIndex - 1].typeKey);
	}
}

void SpawnWindow::drawBody() {
	drawTypesList();
	drawSpawnButton();
}
