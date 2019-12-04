#include "Tile.hpp"

#include <plog/Log.h>

#include <IServer.h>
#include <Resources/ResourceManager.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects.hpp>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

Tile::Tile(Map *map, apos pos) :
    map(map), pos(pos)
{
    uint ux = uint(pos.x);
    uint uy = uint(pos.y);
	icon = IServer::RM()->GetIconInfo("space");
	icon.id += ((ux + uy) ^ ~(ux * uy)) % 25;
}

void Tile::Update(std::chrono::microseconds /*timeElapsed*/) {

}

bool Tile::RemoveObject(Object *obj) {
	if (obj->IsChanged()) {
		auto fieldsDiff = std::make_shared<network::protocol::FieldsDiff>();
		fieldsDiff->objId = obj->ID();
		fieldsDiff->fieldsChanges = obj->PopChanges();
		AddDiff(std::move(fieldsDiff), obj);
	}

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
		auto bumpedTo = lastTile->GetDenseObject(uf::DirectionSet({ direction }));
		if (!bumpedTo)
			bumpedTo = GetDenseObject(uf::DirectionSet({uf::InvertDirection(direction), uf::Direction::CENTER}));
		if (bumpedTo) {
			obj->BumpedTo(bumpedTo);
			return false;
		}
	}

	if (abs(delta.x) > 1 || abs(delta.y) > 1) {
		LOGW << "Warning! Moving more than a one tile. (Tile::MoveTo)";
	}
	if (delta.z) {
		LOGW << "Warning! Moving between Z-levels. (Tile::MoveTo)";
	}

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
	if (lastTile && !obj->GetHolder()) { // if object has holder then it wasn't visible by itself => we don't need to create remove diff
		if (obj->IsChanged()) {
			auto fieldsDiff = std::make_shared<network::protocol::FieldsDiff>();
			fieldsDiff->objId = obj->ID();
			fieldsDiff->fieldsChanges = obj->PopChanges();
			lastTile->AddDiff(std::move(fieldsDiff), obj);
		}
		auto relocateAwayDiff = std::make_shared<network::protocol::RelocateAwayDiff>();
		relocateAwayDiff->objId = obj->ID();
		relocateAwayDiff->newCoords = pos;
		lastTile->AddDiff(relocateAwayDiff, obj);
	} else {
		obj->ResetChanges();
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

Object *Tile::GetDenseObject(uf::DirectionSet directions) const
{
    for (auto &obj : content)
        if (obj->GetSolidity().Rotate(obj->GetDirection()).DoesExistOne(directions)) return obj;
    return nullptr;
}

uf::vec3i Tile::GetPos() const {
    return pos;
}

ITile *Tile::StepTo(uf::Direction direction) const {
	return map->GetTile(GetPos() + uf::DirectionToVect(direction));
}

Map *Tile::GetMap() const { return map; }

bool Tile::IsDense(uf::DirectionSet directions) const {
	for (auto &obj : content)
		if (obj->GetSolidity().Rotate(obj->GetDirection()).DoesExistOne(directions)) return true;
	return false;
}

uf::DirectionSetFractional Tile::GetOpacity() const { return opacity; }
float Tile::GetOpacityTo(uf::Direction direction) const {
	if (!uf::IsPureDirection(direction))
		return 0;

	auto neighbour = map->GetTile(GetPos() + uf::DirectionToVect(direction));
	return GetOpacity().GetCumulativeFraction({direction, uf::Direction::CENTER }) * neighbour->GetOpacity().GetCumulativeFraction({uf::InvertDirection(direction), uf::Direction::CENTER});
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

	opacity += obj->GetOpacity();
	subsystem::atmos::AtmosTile::addObject(obj);

	obj->setTile(this);
	obj->SetSpriteState(Global::ItemSpriteState::DEFAULT);
}

bool Tile::removeObject(Object *obj) {
	if (!obj) 
		return false;
	for (auto iter = content.begin(); iter != content.end(); iter++) {
		if (*iter == obj) {
			obj->setTile(nullptr);
			content.erase(iter);
			opacity -= obj->GetOpacity();
			subsystem::atmos::AtmosTile::removeObject(obj);
			return true;
		}
	}
	return false;
}

void Tile::AddDiff(std::shared_ptr<network::protocol::Diff> diff, Object *obj) {
	EXPECT_WITH_MSG(uf::CreateSerializableById(diff->SerID()), "Send unknown diff!");
	differencesWithObject.push_back({diff, obj->GetOwnershipPointer()});
}

void Tile::ClearDiffs() {
	differencesWithObject.clear();
}
