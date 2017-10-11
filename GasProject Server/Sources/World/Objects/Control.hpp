#pragma once

#include <SFML/System.hpp>

#include "Component.hpp"
#include "Shared/Types.hpp"

class Player;

class Control : public Component {
private:
	float speed;
	uf::vec2i moveIntent;

    // receive from player
	uf::vec2i moveOrder; 
    uint clickedObjectID;

	Player *player;

public:
	explicit Control(float speed);

	void Update(sf::Time timeElapsed) override;

	void MoveCommand(uf::vec2i order);
    void ClickObjectCommand(uint id);

	float GetSpeed() const;
	Player *GetPlayer() const;

	friend Player;
};