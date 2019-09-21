#pragma once

#include <mutex>

#include <Graphics/UI/Widget/ImGuiWidget.h>

#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

class SpawnWindow : public ImGuiWidget {
public:
	void Update(sf::Time timeElapsed) final;

	void UpdateTypes(std::vector<network::protocol::ObjectType> &&types);

private:
	void drawHeader();
	void drawBody();
	void drawTypesList();
	void drawSpawnButton();

private:
	std::vector<network::protocol::ObjectType> types;
	size_t selectedIndex{0};
	std::string searchBuffer;
	std::mutex guard;
	bool noQueriesYet{true};
};
