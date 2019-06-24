#pragma once

#include <SFML/System.hpp>

#include "Component.hpp"
#include "Shared/Types.hpp"

class Player;

class Control : public Component {
public:
    friend Player;

    explicit Control();

    void Update(std::chrono::microseconds timeElapsed) override;

    void MoveCommand(uf::vec2i order);
    void MoveZCommand(bool order);
    void ClickObjectCommand(uint id);

    virtual void SetOwner(Object *owner) override;

    float GetSpeed() const;
    Player *GetPlayer() const;

	uf::vec2i GetAndDropMoveOrder();
	int GetAndDropMoveZOrder();

private:
	float speed;
	// uf::vec2i moveIntent;

    // receive from player
	uf::vec2i moveOrder; 
	int moveZOrder;
    uint clickedObjectID;

	Player *player;
};