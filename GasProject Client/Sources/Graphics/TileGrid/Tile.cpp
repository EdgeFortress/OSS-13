#include "Tile.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Sprite.hpp"
#include "TileGrid.hpp"
#include "Block.hpp"
#include "Object.hpp"

Tile::Tile(Block *block, const int x, const int y) :
	block(block), pos(x, y),
	sprite(nullptr) 
{ };

Tile::~Tile() {
	for (auto &object : content) {
		object->tile = nullptr;
		// TODO: too much cycles. Need to be removed
		block->GetTileGrid()->RemoveObject(object->GetID());
	}
}

void Tile::Draw(sf::RenderTarget *target, uf::vec2i screenPos) const {
	if (sprite) sprite->Draw(target, screenPos, uf::Direction::NONE);
}

void Tile::Update(sf::Time timeElapsed) { }

void Tile::AddObject(Object *obj, int num) {
	if (num < content.size() && num >= 0) {
		auto iter = content.begin();
		for (int i = 0; i < num && iter != content.end(); iter++, i++);
		content.insert(iter, obj);
	}
	else {
		content.push_back(obj);
	}
	if (obj->tile) {
		//int dx = GetRelX() - obj->tile->GetRelX();
		//int dy = GetRelY() - obj->tile->GetRelY();
		//if (obj->shift.x && dx) obj->shift.x -= sgn(dx);
		//if (obj->shift.y && dy) obj->shift.y -= sgn(dy);
		obj->tile->RemoveObject(obj);
	}
	obj->tile = this;
}

Object *Tile::RemoveObject(uint id) {
	for (auto iter = content.begin(); iter != content.end(); iter++)
		if ((*iter)->id == id) {
			Object *obj = (*iter);
			content.erase(iter);
			obj->tile = nullptr;
			return obj;
		}
	return nullptr;
}

Object *Tile::RemoveObject(Object *obj) {
	content.remove(obj);
	obj->tile = nullptr;
	return obj;
}

void Tile::Clear() {
	for (auto &obj : content)
		obj->tile = nullptr;
	content.clear();
	sprite = nullptr;
}

uf::vec2i Tile::GetRelPos() const { return block->GetRelPos() * block->GetTileGrid()->GetBlockSize() + pos; }

Object *Tile::GetObjectByPixel(uf::vec2i pixel) const {
	for (auto obj = content.rbegin(); obj != content.rend(); obj++) {
		if ((*obj)->checkObj(pixel.x, pixel.y))
			return *obj;
	}
	return nullptr;
}

Object *Tile::GetObject(uint id) {
	for (auto iter = content.begin(); iter != content.end(); iter++)
		if ((*iter)->id == id)
			return *iter;
	return nullptr;
}

TileGrid *Tile::GetTileGrid() {
    return block->GetTileGrid();
}

bool Tile::IsBlocked() {
	for (auto &obj : content)
		if (obj->IsDense()) return true;
	if (!sprite) return true;
	return false;
}