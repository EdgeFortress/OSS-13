#pragma once

#include <SFML/System.hpp>

#include <Shared/Types.hpp>

#include "Component.hpp"
#include "ControlUI.h"

class Player;

class Control : public Component {
public:
    friend Player;

    explicit Control();

    void Update(std::chrono::microseconds timeElapsed) override;

    void MoveCommand(uf::vec2i order);
    void MoveZCommand(bool order);
    void ClickObjectCommand(uint id);
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

private:
	float speed;
	uint camera_seeInvisibleAbility{0}; // crutch. TODO: divide camera and control logics

    // receive from player
	uf::vec2i moveOrder; 
	int moveZOrder;
	uint clickedObjectID{0};

	Player *player{nullptr};
	std::unique_ptr<ControlUI> ui;
};
