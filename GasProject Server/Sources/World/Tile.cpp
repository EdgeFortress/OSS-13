#include "Tile.hpp"

#include <Server.hpp>
#include <Network/Differences.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects.hpp>
#include <World/Atmos/Atmos.hpp>

Tile::Tile(Map *map, uf::vec2i pos) :
    map(map), pos(pos),
    hasFloor(false), fullBlocked(false), directionsBlocked(4, false),
    locale(nullptr), needToUpdateLocale(false), gases(int(Gas::Count), 0)
{
    uint ux = uint(pos.x);
    uint uy = uint(pos.y);
	icon = Server::Get()->RM->GetIconInfo("space");
	icon.id += ((ux + uy) ^ ~(ux * uy)) % 25;

    totalPressure = 0;
}

void Tile::Update(sf::Time timeElapsed) {
    // Update locale, if wall/floor state was changed
    if (needToUpdateLocale) {
        // Atmos-available tile
        if (hasFloor && !fullBlocked) {
            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++) {
                    Tile *neighbour = map->GetTile(pos + uf::vec2i(dx, dy));
                    if (!neighbour ||
                        dx == 0 && dy == 0 ||
                        dx * dy != 0) // diag tiles
                        continue;
                    if (neighbour->locale) {
                        if (locale) {
                            locale->Merge(neighbour->locale);
                        } else {
                            neighbour->locale->AddTile(this);
                            locale = neighbour->locale;
                        }
                    }
                }
            if (!locale) {
                map->GetAtmos()->CreateLocale(this);
            }
        } else { // Space
            if (!hasFloor && !fullBlocked) {
                for (int dx = -1; dx <= 1; dx++)
                    for (int dy = -1; dy <= 1; dy++) {
                        Tile *neighbour = map->GetTile(pos + uf::vec2i(dx, dy));
                        if (!neighbour ||
                            dx == 0 && dy == 0 ||
                            dx * dy != 0) // diag tiles
                            continue;
                        if (neighbour->locale) {
                            neighbour->locale->Open();
                        }
                    }
                if (locale) locale->RemoveTile(this);
            } else { // fullBlocked
                // if here was locale then remove it
                if (locale) { 
                    map->GetAtmos()->RemoveLocale(locale);
                }
                // if here was a space then we need to update neighbors locals 
                // so we delete them, after that Atmos::Update recreate them
                for (int dx = -1; dx <= 1; dx++)
                    for (int dy = -1; dy <= 1; dy++) {
                        Tile *neighbour = map->GetTile(pos + uf::vec2i(dx, dy));
                        if (!neighbour ||
                            dx == 0 && dy == 0 ||
                            dx * dy != 0) // diag tiles
                            continue;
                        if (neighbour->locale) {
                            neighbour->locale->CheckCloseness();
                        }
                    }
            }
        }
        needToUpdateLocale = false;
    }
}

void Tile::CheckLocale() {
    needToUpdateLocale = true;
}

bool Tile::RemoveObject(Object *obj) {
    if (removeObject(obj)) {
        GetBlock()->AddDiff(new RemoveDiff(obj));
        return true;
    }
    return false;
}

bool Tile::MoveTo(Object *obj) {
    if (!obj) return false;

    if (!obj->IsMovable()) {
        Server::log << "Warning! Try to move immovable object" << std::endl;
        return false;
    }

    if (obj->GetDensity())
        for (auto &object : content)
            if (object)
                if (object->GetDensity()) {
                    return false;
                }

    Block *lastBlock = obj->GetTile()->GetBlock();
    uf::vec2i delta = GetPos() - obj->GetTile()->GetPos();
    if (abs(delta.x) > 1 || abs(delta.y) > 1)
        Server::log << "Warning! Moving more than a one tile. (Tile::MoveTo)" << std::endl;
    const uf::Direction direction = uf::VectToDirection(delta);
    addObject(obj);
    GetBlock()->AddDiff(new MoveDiff(obj, direction, obj->GetMoveSpeed(), lastBlock));

    return true;
}

void Tile::PlaceTo(Object *obj) {
    Tile *lastTile = obj->GetTile();
    Block *lastBlock = nullptr;
    if (lastTile) lastBlock = lastTile->GetBlock();

    // If obj is wall or floor - remove previous and change status
    if (dynamic_cast<Floor *>(obj)) {
        if (hasFloor) {
            for (auto iter = content.begin(); iter != content.end(); iter++) {
                if (dynamic_cast<Floor *>(*iter)) {
                    content.erase(iter);
                    break;
                }
            }
        }
        hasFloor = true;
        CheckLocale();
    } else if (dynamic_cast<Wall *>(obj)) {
        if (!hasFloor) {
            Server::log << "Warning! Try to place wall without floor" << std::endl;
            return;
        }
        if (fullBlocked) {
            for (auto iter = content.begin(); iter != content.end(); iter++) {
                if (dynamic_cast<Wall *>(*iter)) {
                    content.erase(iter);
                    break;
                }
            }
            
        }
        fullBlocked = true;
        CheckLocale();
    }

    addObject(obj);
    GetBlock()->AddDiff(new ReplaceDiff(obj, pos.x, pos.y, lastBlock));
}

const list<Object *> &Tile::Content() const {
    return content;
}

Object *Tile::GetDenseObject() const
{
    for (auto &obj : content)
        if (obj->GetDensity()) return obj;
    return nullptr;
}

uf::vec2i Tile::GetPos() const {
    return pos;
}

Block *Tile::GetBlock() const {
    return map->GetBlock(pos / Global::BLOCK_SIZE);
}

Map *Tile::GetMap() const { return map; }

bool Tile::IsDense() const {
    for (auto &obj : content)
        if (obj->GetDensity()) return true;
    return false;
}

bool Tile::IsSpace() const {
    return !hasFloor && !fullBlocked;
}

const TileInfo Tile::GetTileInfo(uint visibility) const {
	TileInfo tileInfo;
	tileInfo.sprite = icon.id;

	for (auto &obj : this->content) {
		if (obj->CheckVisibility(visibility))
			tileInfo.content.push_back(obj->GetObjectInfo());
	}

    return tileInfo;
}

void Tile::addObject(Object *obj) {
    Tile *lastTile = obj->GetTile();
    if (lastTile) lastTile->removeObject(obj);

    // Count position in tile content by layer
    auto iter = content.begin();
    while (iter != content.end() && (*iter)->GetLayer() < obj->GetLayer())
        iter++;
    content.insert(iter, obj);

	obj->setTile(this);
}

bool Tile::removeObject(Object *obj) {
    for (auto iter = content.begin(); iter != content.end(); iter++) {
        if (*iter == obj) {
            if (dynamic_cast<Floor *>(obj)) {
                hasFloor = false;
                CheckLocale();
            } else if (dynamic_cast<Wall *>(obj)) {
                fullBlocked = false;
                CheckLocale();
            }
            obj->setTile(nullptr);
            content.erase(iter);
            return true;
        }
    }
    return false;
}