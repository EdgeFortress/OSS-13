#include "Tile.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Sprite.hpp"
#include "TileGrid.hpp"
#include "Object.hpp"

#include <Graphics/UI/UI.hpp>

Tile::Tile(TileGrid *tileGrid) :
	tileGrid(tileGrid)
{ 
	overlay.setFont(CC::Get()->GetUI()->GetFont());
	overlay.setCharacterSize(10);
}

Tile::Tile(TileGrid *tileGrid, const network::protocol::TileInfo &tileInfo) :
	Tile(tileGrid)
{
	sprite = CC::Get()->RM.CreateSprite(uint(tileInfo.sprite));

	for (auto &objInfo : tileInfo.content) {
		auto &objects = GetTileGrid()->GetObjects();

		auto iter = objects.find(objInfo.id);
		if (iter == objects.end()) {
			objects[objInfo.id] = std::make_unique<Object>(objInfo);
		}

		AddObject(objects[objInfo.id].get());
	}
	Resize(GetTileGrid()->GetTileSize());
}

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

	if (num < static_cast<int>(content.size()) && num >= 0) {
		auto iter = content.begin();
		for (int i = 0; i < num && iter != content.end(); iter++, i++){}
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

apos Tile::GetRelPos() const { return relPos; }

Object *Tile::GetObject(uint id) {
	for (auto iter = content.begin(); iter != content.end(); iter++)
		if ((*iter)->id == id)
			return *iter;
	return nullptr;
}

TileGrid *Tile::GetTileGrid() {
    return tileGrid;
}

bool Tile::IsBlocked() const {
	for (auto &obj : content)
		if (obj->IsDense()) return true;
	if (!sprite.IsValid()) return true;
	return false;
}

bool Tile::IsBlocked(uf::DirectionSet directions) const {
	for (auto &obj : content)
		if (obj->GetSolidity().DoesExistOne(directions)) return true;
	return false;
}
