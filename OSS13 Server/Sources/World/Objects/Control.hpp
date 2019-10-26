#pragma once

#include <SFML/System.hpp>

#include <Shared/Types.hpp>

#include "Component.hpp"
#include "ControlUI.h"
#include "../Tile.hpp"

class Player;

class Control : public Component {
public:
	friend Player;

	explicit Control();

	void Update(std::chrono::microseconds timeElapsed) override;

	void MoveCommand(uf::vec2i order);
	void MoveZCommand(bool order);
	void ClickObjectCommand(uint id);
	void ClickTileCommand(uf::vec3i pos);
	void ClickUICommand(const std::string &key);

	virtual void SetOwner(Object *owner) override;

	uint GetSeeInvisibleAbility() const;
	void SetSeeInvisibleAbility(uint flags);

	float GetSpeed() const;
	Player *GetPlayer() const;
	ControlUI *GetUI() const;

	uf::vec2i GetAndDropMoveOrder();
	int GetAndDropMoveZOrder();
	Object *GetAndDropClickedObject();
	Tile *GetAndDropClickedTile();

private:
	float speed{4};
	uint camera_seeInvisibleAbility{}; // crutch. TODO: divide camera and control logics

	// received from player
	uf::vec2i moveOrder; 
	int moveZOrder{};
	uint clickedObjectID{};
	uf::vec3i clickedTilePos;
	bool isTileClicked;

	Player *player{nullptr};
	std::unique_ptr<ControlUI> ui;
};
