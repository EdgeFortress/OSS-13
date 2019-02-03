#pragma once

#include <string>

#include <Graphics/Sprite.hpp>

#include <Shared/Types.hpp>
#include <Shared/Geometry/Direction.hpp>
#include <Shared/Global.hpp>

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
    ~Object();

	void Draw(sf::RenderTarget *, uf::vec2i windowPos);
	void Update(sf::Time timeElapsed);
    void Resize(uint tileSize);

    void SetID(uint id) { this->id = id; }
	void AddSprite(uint spriteID);
	void ClearSprites();
    void PlayAnimation(uint spriteID);
	void SetDirection(const uf::Direction);
	void SetMoveSpeed(float speed);
	void SetMoveIntent(uf::vec2i moveIntent);
	void ResetShiftingState();
	void ReverseShifting(uf::Direction direction);

	uint GetID() const;
	std::string GetName() const;
	//Sprite *GetSprite() const;
	uint GetLayer() const;
    bool PixelTransparent(uf::vec2i pixel) const;

	Tile *GetTile();
	sf::Vector2f GetShift() const;
    sf::Vector2i GetMoveIntent() const;
	//sf::Vector2i GetShiftingDirection() const;
	bool IsDense();

	friend sf::Packet &operator>>(sf::Packet &packet, Object &object);
	friend Tile;

private:
    uint id;
    std::string name;
	std::list<::Sprite> sprites;
    ::Sprite animation;
    bool animationProcess;
    uf::Direction direction;
    uint layer;
    bool dense;

    uf::vec2f shift;
    
    float moveSpeed;
    uf::vec2i moveIntent;
    uf::vec2f constSpeed;  
    uf::vec2f physSpeed;

    Tile *tile;
};