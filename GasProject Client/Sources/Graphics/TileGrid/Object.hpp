#pragma once

#include <string>

#include "Shared/Types.hpp"
#include "Shared/Geometry/Direction.hpp"
#include "Shared/Global.hpp"

class Sprite;
class Tile;

namespace sf {
	class RenderTarget;
	class Time;
	class Packet;
}

class Object {
private:
	uint id;
	std::string name;
	Sprite *sprite;
	uf::Direction direction;
	int layer;
	bool dense;

	sf::Vector2f shift;
	sf::Vector2i shiftingDirection;
	float shiftingSpeed;

	Tile *tile;

public:
	explicit Object(uint spriteID = 0, const std::string &name = "");

	void Draw(sf::RenderTarget *, const int x, const int y);
	void Update(sf::Time timeElapsed);

	void SetSprite(uint spriteID);
	void SetDirection(const uf::Direction);
	void SetSpeed(float speed);
	void SetShifting(uf::Direction direction, float speed);
	void ResetShifting();
	void ReverseShifting(uf::Direction direction);

	bool checkObj(int x, int y);

	uint GetID() const;
	std::string GetName() const;
	Sprite *GetSprite() const;
	int GetLayer() const;

	Tile *GetTile();
	sf::Vector2f GetShift() const;
	sf::Vector2i GetShiftingDirection() const;
	bool IsDense();

	friend sf::Packet &operator>>(sf::Packet &packet, Object &object);
	friend Tile;
};