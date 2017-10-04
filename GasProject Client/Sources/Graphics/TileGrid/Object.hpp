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
public:
	Object();
    Object(const Object &) = delete;
    Object &operator=(const Object &) = delete;
    ~Object() = default;

	void Draw(sf::RenderTarget *, uf::vec2i windowPos);
	void Update(sf::Time timeElapsed);

	void SetSprite(uint spriteID);
	void SetDirection(const uf::Direction);
	void SetSpeed(float speed);
	void SetShifting(uf::Direction direction, float speed);
	void ResetShifting();
	void ReverseShifting(uf::Direction direction);

    // TODO: Rename
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

private:
    uint id;
    std::string name;
    Sprite *sprite;
    uf::Direction direction;
    int layer;
    bool dense;

    uf::vec2f shift;
    uf::vec2i shiftingDirection;
    float shiftingSpeed;

    Tile *tile;
};