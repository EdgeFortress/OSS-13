#include "Tile.hpp"

#include <plog/Log.h>

#include <IServer.h>
#include <Resources/ResourceManager.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects.hpp>
#include <World/Atmos/Atmos.hpp>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

Tile::Tile(Map *map, apos pos) :
    map(map), pos(pos),
    hasFloor(false), fullBlocked(false), directionsBlocked(4, false),
    locale(nullptr), needToUpdateLocale(false), gases(int(Gas::Count), 0)
{
    uint ux = uint(pos.x);
    uint uy = uint(pos.y);
	icon = IServer::RM()->GetIconInfo("space");
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
		auto diff = std::make_shared<network::protocol::RemoveDiff>();
		diff->objId = obj->ID();
		AddDiff(diff, obj);
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

	Tile *lastTile = obj->GetTile();
	rpos delta = GetPos() - lastTile->GetPos();
	uf::Direction direction = uf::VectToDirection(delta);

	if (obj->GetDensity()) {
		auto bumpedTo = lastTile->GetDenseObject(DirectionSet({ direction }));
		if (!bumpedTo)
			bumpedTo = GetDenseObject(DirectionSet({uf::InvertDirection(direction), uf::Direction::CENTER}));
		if (bumpedTo) {
			obj->BumpedTo(bumpedTo);
			return false;
		}
	}

	if (abs(delta.x) > 1 || abs(delta.y) > 1)
		LOGW << "Warning! Moving more than a one tile. (Tile::MoveTo)";
	if (delta.z)
		LOGW << "Warning! Moving between Z-levels. (Tile::MoveTo)";

	auto relocateAwayDiff = std::make_shared<network::protocol::RelocateAwayDiff>(); // TODO: MoveAway???
	relocateAwayDiff->objId = obj->ID();
	relocateAwayDiff->newCoords = pos;

	lastTile->AddDiff(std::move(relocateAwayDiff), obj);

	addObject(obj);

	auto moveDiff = std::make_shared<network::protocol::MoveDiff>();
	moveDiff->objId = obj->ID();
	moveDiff->direction = direction;
	moveDiff->speed = obj->GetMoveSpeed();

	AddDiff(moveDiff, obj);

	return true;
}

void Tile::PlaceTo(Object *obj) {
	if (!obj)
		return;

	Tile *lastTile = obj->GetTile();

	// If obj is wall or floor - remove previous and change status
	if (obj->IsFloor()) {
		if (hasFloor) {
			for (auto iter = content.begin(); iter != content.end(); iter++) {
				if ((*iter)->IsFloor()) {
					content.erase(iter);
					break;
				}
			}
		}
		hasFloor = true;
		CheckLocale();
	} else if (obj->IsWall()) {
		if (!hasFloor) {
			LOGW << "Warning! Try to place wall without floor";
			return;
		}
		if (fullBlocked) {
			for (auto iter = content.begin(); iter != content.end(); iter++) {
				if ((*iter)->IsWall()) {
					content.erase(iter);
					break;
				}
			}
            
		}
		fullBlocked = true;
		CheckLocale();
	}

	auto objInfo = obj->GetObjectInfo();

	if (lastTile) {
		auto relocateAwayDiff = std::make_shared<network::protocol::RelocateAwayDiff>();
		relocateAwayDiff->objId = obj->ID();
		relocateAwayDiff->newCoords = pos;
		lastTile->AddDiff(relocateAwayDiff, obj);
	}
	addObject(obj);

	auto relocateDiff = std::make_shared<network::protocol::RelocateDiff>();
	relocateDiff->objId = obj->ID();
	relocateDiff->newCoords = pos;
	AddDiff(relocateDiff, obj);
}

const std::list<Object *> &Tile::Content() const {
    return content;
}

Object *Tile::GetDenseObject(DirectionSet directions) const
{
    for (auto &obj : content)
        if (obj->GetSolidity().IsExistsOne(directions)) return obj;
    return nullptr;
}

uf::vec3i Tile::GetPos() const {
    return pos;
}

Map *Tile::GetMap() const { return map; }

bool Tile::IsDense(uf::DirectionSet directions) const {
	for (auto &obj : content)
		if (obj->GetSolidity().IsExistsOne(directions)) return true;
	return false;
}

bool Tile::IsSpace() const {
    return !hasFloor && !fullBlocked;
}

Locale *Tile::GetLocale() const {
	return locale;
}

network::protocol::TileInfo Tile::GetTileInfo(uint viewerId, uint visibility) const {
	network::protocol::TileInfo tileInfo;
	tileInfo.coords = pos;
	tileInfo.sprite = icon.id;

	for (auto &obj : this->content) {
		if (obj->CheckVisibility(viewerId, visibility))
			tileInfo.content.push_back(obj->GetObjectInfo());
	}

    return tileInfo;
}

void Tile::addObject(Object *obj) {
	if (!obj)
		return;

	Object *holder = obj->GetHolder();
	if (holder) {
		EXPECT(holder->RemoveObject(obj));
	}

	Tile *lastTile = obj->GetTile();
	if (lastTile) {
		EXPECT(lastTile->removeObject(obj));
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
	if (!obj) 
		return false;
	for (auto iter = content.begin(); iter != content.end(); iter++) {
		if (*iter == obj) {
			if (obj->IsFloor()) {
				hasFloor = false;
				CheckLocale();
			} else if (obj->IsWall()) {
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

void Tile::AddDiff(std::shared_ptr<network::protocol::Diff> diff, Object *obj) {
	EXPECT(uf::CreateSerializableById(diff->Id())); // debug
	differencesWithObject.push_back({diff, obj->GetOwnershipPointer()});
}

void Tile::ClearDiffs() {
	differencesWithObject.clear();
}
