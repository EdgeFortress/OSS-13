#include "Object.hpp"

#include "Shared/Math.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/UI/UI.hpp"
#include "Graphics/UI/UIModule/GameProcessUI.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/TileGrid.hpp"

Object::Object(const Global::Sprite key, const std::string name, const bool directed) {
	SetSprite(key);
	this->name = name;
	direction = uf::Direction::NONE;
	dense = false;
}

void Object::SetSprite(const Global::Sprite key) {
	sprite = nullptr;
	if (int(key))
		for (auto &sprite : CC::Get()->GetWindow()->GetSprites())
			if (sprite->GetKey() == key) {
				this->sprite = sprite.get();
				return;
			}
}

void Object::SetDirection(const uf::Direction direction) {
	this->direction = direction;
}

void Object::SetSpeed(float speed) {
	shiftingSpeed = speed;
}

void Object::SetShifting(uf::Direction direction, float speed) {
	auto directionVect = sf::Vector2i(uf::DirectionToVect(direction));
	if (shiftingDirection != directionVect) shiftingDirection += directionVect;
	shiftingSpeed = speed;
}

void Object::ResetShifting() {
	shiftingDirection = sf::Vector2i(0, 0);
	shift = sf::Vector2f(0, 0);
	shiftingSpeed = 0;
}

bool Object::checkObj(int x, int y) {
	return !(sprite->PixelTransparent(x, y));
}

void Object::Draw(sf::RenderTarget *target, const int x, const int y) {
	// TODO: simplify
	int tileSize = reinterpret_cast<GameProcessUI *>(CC::Get()->GetUI()->GetCurrentUIModule())->GetTileGrid()->GetTileSize();
	if (sprite) sprite->Draw(target, x + int(shift.x * tileSize), y + int(shift.y * tileSize), direction);
}

void Object::Update(sf::Time timeElapsed) {
	if (shiftingDirection.x) {
		shift.x += shiftingDirection.x * timeElapsed.asSeconds() * shiftingSpeed;
	}
	else {
		if (shift.x * uf::sgn(shift.x) > timeElapsed.asSeconds() * shiftingSpeed)
			shift.x -= uf::sgn(shift.x) * timeElapsed.asSeconds() * shiftingSpeed;
		else shift.x = 0;
	}
	if (shiftingDirection.y) {
		shift.y += shiftingDirection.y * timeElapsed.asSeconds() * shiftingSpeed;
	}
	else {
		if (shift.y * uf::sgn(shift.y) > timeElapsed.asSeconds() * shiftingSpeed)
			shift.y -= uf::sgn(shift.y) * timeElapsed.asSeconds() * shiftingSpeed;
		else shift.y = 0;
	}
}

void Object::ReverseShifting(uf::Direction direction) {
	sf::Vector2i directionVect = uf::DirectionToVect(direction);
	if (directionVect.x) shiftingDirection.x = 0;
	if (directionVect.y) shiftingDirection.y = 0;
	shift -= sf::Vector2f(directionVect);
}

uint Object::GetID() const { return id; }
std::string Object::GetName() const { return name; }
Sprite *Object::GetSprite() const { return sprite; }
int Object::GetLayer() const { return layer; }

Tile *Object::GetTile() { return tile; }
sf::Vector2f Object::GetShift() const { return shift; }
sf::Vector2i Object::GetShiftingDirection() const { return shiftingDirection; }
bool Object::IsDense() { return dense; }