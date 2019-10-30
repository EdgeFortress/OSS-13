#pragma once

#include <vector>

#include <SFML/System.hpp>

#include <Resources/IconInfo.h>
#include <World/ITile.h>
#include <World/Subsystems/IAtmos.h>
#include <World/Subsystems/Atmos/AtmosTile.h>

#include <Shared/Global.hpp>
#include <Shared/Types.hpp>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>
#include <Shared/Network/Protocol/ServerToClient/Diff.h>

class Tile : public subsystem::atmos::AtmosTile {
public:
	Tile(Map *map, apos pos);

	void Update(std::chrono::microseconds timeElapsed) final;

	bool RemoveObject(Object *obj) final;
	bool MoveTo(Object *) final;
	void PlaceTo(Object *) final;

	const std::list<Object *> &Content() const final;

	uf::vec3i GetPos() const final;
	ITile *StepTo(uf::Direction direction) const final;
	Map *GetMap() const final;

	bool IsDense(uf::DirectionSet directions) const;
	Object *GetDenseObject(uf::DirectionSet directions) const final;

	uf::DirectionSetFractional GetOpacity() const;
	float GetOpacityTo(uf::Direction direction) const;

	network::protocol::TileInfo GetTileInfo(uint viewerId, uint visibility) const;

	void AddDiff(std::shared_ptr<network::protocol::Diff> diff, Object *object);
	const std::vector<std::pair<std::shared_ptr<network::protocol::Diff>, sptr<Object>>> &GetDifferencesWithObject() const { return differencesWithObject; }
	void ClearDiffs();

	int X() const { return pos.x; }
	int Y() const { return pos.y; }
	int Z() const { return pos.z; }

private:
	void addObject(Object *obj) final;
	bool removeObject(Object *obj) final;

private:
	Map *map;
	uf::vec3i pos;

	IconInfo icon;

	std::list<Object *> content;
	uf::DirectionSetFractional opacity;

	std::vector<std::pair<std::shared_ptr<network::protocol::Diff>, sptr<Object>>> differencesWithObject;
};
