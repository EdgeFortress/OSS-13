#include "TileContextMenu.h"

#include <imgui.h>

#include <Client.hpp>
#include <Network/Connection.h>

#include <Shared/Network/Protocol/ClientToServer/Commands.h>

void TileContextMenu::Update(sf::Time timeElapsed) {
	std::unique_lock<std::mutex> lock(guard);

	openPopupIfNeeded();
	drawContent();
}

void TileContextMenu::Open() {
	askedOpen = true;
}

void TileContextMenu::SetContent(network::protocol::ContextMenuData &&data) {
	std::unique_lock<std::mutex> lock(guard);

	this->data = std::make_unique<network::protocol::ContextMenuData>(std::move(data));
	dataUpdated = true;
}

void TileContextMenu::openPopupIfNeeded() {
	if (askedOpen && dataUpdated) {
		askedOpen = false;
		dataUpdated = false;
		ImGui::OpenPopup("TileContextMenu");
	}
}

void drawNode(const network::protocol::ContextMenuNode &node, uint8_t nodeIndex) {
	if (ImGui::BeginMenu(node.title.c_str())) {
		uint8_t verbsCounter = 0;
		for (auto &verb : node.verbs) {
			if (ImGui::MenuItem(verb.c_str())) {
				auto command = std::make_unique<network::protocol::client::ContextMenuClickCommand>();
				command->node = nodeIndex;
				command->verb = verbsCounter;
				Connection::commandQueue.Push(command.release());
			}
			verbsCounter++;
		}
		ImGui::EndMenu();
	}
}

void TileContextMenu::drawContent() {
	if (ImGui::BeginPopup("TileContextMenu")) {
		uint8_t nodeCounter = 0;
		for (auto &node : data->nodes)
			drawNode(*node, nodeCounter++);
		ImGui::EndPopup();
	}
}
