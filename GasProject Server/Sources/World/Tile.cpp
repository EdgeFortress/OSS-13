#include "Tile.hpp"

#include <plog/Log.h>

#include <IServer.h>
#include <Resources/ResourceManager.hpp>
#include <Network/Differences.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects.hpp>
#include <World/Atmos/Atmos.hpp>

Tile::Tile(Map *map, apos pos) :
    map(map), pos(pos),
    hasFloor(false), fullBlocked(false), directionsBlocked(4, false),
    locale(nullptr), needToUpdateLocale(false), gases(int(Gas::Count), 0)
{
    uint ux = uint(pos.x);
    uint uy = uint(pos.y);
	icon = GServer->GetRM()->GetIconInfo("space");
	icon.id += ((ux + uy) ^ ~(ux * uy)) % 25;

    totalPressure = 0;
}

void Tile::Update(std::chrono::microseconds timeElapsed) {
    // Update locale, if wall/floor state was changed
    if (needToUpdateLocale) {
        // Atmos-available tile
        if (hasFloor && !fullBlocked) {
            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++) {
                    Tile *neighbour = map->GetTile(pos + rpos(dx, dy, 0));
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
                        Tile *neighbour = map->GetTile(pos + rpos(dx, dy, 0));
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
                        Tile *neighbour = map->GetTile(pos + rpos(dx, dy, 0));
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
        AddDiff(new RemoveDiff(obj));
        return true;
    }
    return false;
}

bool Tile::MoveTo(Object *obj) {
    if (!obj) return false;

    if (!obj->IsMovable()) {
        LOGW << "Warning! Try to move immovable object";
        return false;
    }

    if (obj->GetDensity())
        for (auto &object : content)
            if (object)
                if (object->GetDensity()) {
                    return false;
                }

    Tile *lastTile = obj->GetTile();
    rpos delta = GetPos() - lastTile->GetPos();
    if (abs(delta.x) > 1 || abs(delta.y) > 1)
        LOGW << "Warning! Moving more than a one tile. (Tile::MoveTo)";
    if (delta.z)
        LOGW << "Warning! Moving between Z-levels. (Tile::MoveTo)";
    const uf::Direction direction = uf::VectToDirection(delta);
    addObject(obj);
    AddDiff(new MoveDiff(obj, direction, obj->GetMoveSpeed(), lastTile));

    return true;
}

void Tile::PlaceTo(Object *obj) {
	if (!obj)
		return;

    Tile *lastTile = obj->GetTile();

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
            LOGW << "Warning! Try to place wall without floor";
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
    AddDiff(new ReplaceDiff(obj, pos.x, pos.y, pos.z, lastTile));
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

apos Tile::GetPos() const {
    return pos;
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

Locale *Tile::GetLocale() const {
	return locale;
}

const TileInfo Tile::GetTileInfo(uint visibility) const {
	TileInfo tileInfo;
	tileInfo.x = pos.x;
	tileInfo.y = pos.y;
	tileInfo.z = pos.z;
	tileInfo.sprite = icon.id;

	for (auto &obj : this->content) {
		if (obj->CheckVisibility(visibility))
			tileInfo.content.push_back(obj->GetObjectInfo());
	}

    return tileInfo;
}

void Tile::addObject(Object *obj) {
	if (!obj)
		return;

	Object *holder = obj->GetHolder();
	if (holder) {
		if (!holder->RemoveObject(obj))
			throw std::exception(); // "Unexpected!"
	}

	Tile *lastTile = obj->GetTile();
	if (lastTile) {
		if (!lastTile->removeObject(obj))
			throw std::exception(); // "Unexpected!"
	}

    // Count position in tile content by layer
    auto iter = content.begin();
    while (iter != content.end() && (*iter)->GetLayer() <= obj->GetLayer())
        iter++;
    content.insert(iter, obj);

	obj->setTile(this);
	obj->SetSpriteState(Global::ItemSpriteState::DEFAULT);
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

void Tile::AddDiff(Diff *diff) {
    differences.push_back(sptr<Diff>(diff));
}

void Tile::ClearDiffs() {
    differences.clear();
}
