#pragma once

#include <SFML/System.hpp>

class Controllable : public Object {
private:
	sf::Vector2i moveOrder;

protected:
	sf::Vector2i getMoveOrder() {
		sf::Vector2i tmp = moveOrder;
		moveOrder = sf::Vector2i();
		return tmp;
	}

public:
	Controllable() : moveOrder(0, 0) {

	}
	void MoveCommand(sf::Vector2i order) {
		moveOrder = order;
	}
	Tile *GetTile() const { return tile; }
};