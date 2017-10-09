#pragma once

#include <SFML/System.hpp>

#include "Component.hpp"
#include "Shared/Types.hpp"

class Player;

class Control : public Component {
private:
	float speed;
	uf::vec2i moveIntent;
	uf::vec2i moveOrder; // receive from player

	Player *player;

public:
	explicit Control(float speed);

	void Update(sf::Time timeElapsed) override;

	void MoveCommand(uf::vec2i order);

	float GetSpeed() const;
	Player *GetPlayer() const;

	friend Player;
};