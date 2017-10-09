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

#include "iostream"

class Object {
public:
	Object();
    Object(const Object &) = default;
    Object &operator=(Object &) = default;
    ~Object() {  }

	void Draw(sf::RenderTarget *, uf::vec2i windowPos);
	void Update(sf::Time timeElapsed);

    void SetID(uint id) { this->id = id; }
	void SetSprite(uint spriteID);
	void SetDirection(const uf::Direction);
	void SetSpeed(float speed);
	void SetShifting(uf::Direction direction, float speed);
	void ResetShifting();
	void ReverseShifting(uf::Direction direction);

	uint GetID() const;
	std::string GetName() const;
	Sprite *GetSprite() const;
	uint GetLayer() const;
    bool PixelTransparent(uf::vec2i pixel);

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
    uint layer;
    bool dense;

    uf::vec2f shift;
    uf::vec2i shiftingDirection;
    float shiftingSpeed;

    Tile *tile;
};