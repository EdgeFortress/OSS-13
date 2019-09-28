#pragma once

#include <Graphics/UI/Widget/ImGuiWidget.h>

#include <Shared/Network/Protocol/ServerToClient/ContextMenuData.h>

class TileContextMenu : public ImGuiWidget {
public:
	void Update(sf::Time timeElapsed) final;

	void Open();
	void SetContent(network::protocol::ContextMenuData &&data);

private:
	void openPopupIfNeeded();
	void drawContent();

private:
	bool askedOpen{false};
	bool dataUpdated{false};
	std::unique_ptr<network::protocol::ContextMenuData> data;
};
