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
	}
}

void Tile::Draw(sf::RenderTarget *target, uf::vec2i screenPos) const {
	if (sprite) sprite->Draw(target, screenPos, uf::Direction::NONE);
}

void Tile::Update(sf::Time timeElapsed) { }

void Tile::AddObject(Object *obj, int num) {
    if (obj->tile) {
        obj->tile->RemoveObject(obj);
    }
    obj->tile = this;

	if (num < content.size() && num >= 0) {
		auto iter = content.begin();
		for (int i = 0; i < num && iter != content.end(); iter++, i++);
		    content.insert(iter, obj);
	} else {
		content.push_back(obj);
	}
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