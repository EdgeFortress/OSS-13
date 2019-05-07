#include "Tile.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Sprite.hpp"
#include "TileGrid.hpp"
#include "Block.hpp"
#include "Object.hpp"

#include <Graphics/UI/UI.hpp>

Tile::Tile(Block *block, const int x, const int y) :
	block(block), pos(x, y)
{ 
	overlay.setFont(CC::Get()->GetUI()->GetFont());
	overlay.setCharacterSize(10);
};

Tile::~Tile() {
	for (auto &object : content) {
		object->tile = nullptr;
	}
}

void Tile::Draw(sf::RenderTarget *target, uf::vec2i screenPos) const {
	if (sprite.IsValid()) sprite.Draw(target, screenPos);
}

void Tile::DrawOverlay(sf::RenderTarget *target, uf::vec2i screenPos) const {
	overlay.setPosition(screenPos);
	target->draw(overlay);
}

void Tile::Update(sf::Time timeElapsed) { 
    if (sprite.IsValid())
        sprite.Update(timeElapsed);
}

void Tile::Resize(uint tileSize) {
    if (sprite.IsValid())
        sprite.Resize(tileSize);
    for (auto &object : content)
        object->Resize(tileSize);
}

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
}

void Tile::SetOverlay(std::string text) {
	overlay.setString(text);
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
	if (!sprite.IsValid()) return true;
	return false;
}