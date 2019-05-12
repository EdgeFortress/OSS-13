#pragma once

#include <SFML/System.hpp>

#include "Component.hpp"
#include "Shared/Types.hpp"

class Player;

class Control : public Component {
public:
    friend Player;

    explicit Control(float speed);

    void Update(sf::Time timeElapsed) override;

    void MoveCommand(uf::vec2i order);
    void MoveZCommand(bool order);
    void ClickObjectCommand(uint id);

    virtual void SetOwner(Object *owner) override;

    float GetSpeed() const;
    Player *GetPlayer() const;

private:
	float speed;
	// uf::vec2i moveIntent;

    // receive from player
	uf::vec2i moveOrder; 
	int moveZOrder;
    uint clickedObjectID;

	Player *player;
};