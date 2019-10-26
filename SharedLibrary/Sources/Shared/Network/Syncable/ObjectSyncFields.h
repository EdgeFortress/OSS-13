#pragma once

#include <Shared/Network/Archive.h>
#include <Shared/Network/Syncable.h>
#include <Shared/Geometry/DirectionSet.h>

namespace network {
namespace sync {

DEFINE_SYNCABLE(ObjectSyncFields)
protected:
	uf::SyncField<std::string> name;
	uf::SyncField<uf::Direction> direction;
	uf::SyncField<uf::DirectionSet> solidity;
	uf::SyncField<bool> drawAtTop;

public:
	ObjectSyncFields() { 
		DECLARE_SYNCABLE_FIELD(&ObjectSyncFields::name);
		DECLARE_SYNCABLE_FIELD(&ObjectSyncFields::direction);
		DECLARE_SYNCABLE_FIELD(&ObjectSyncFields::solidity);
		DECLARE_SYNCABLE_FIELD(&ObjectSyncFields::drawAtTop);
	};
DEFINE_SYNCABLE_END 

} // namespace sync
} // namespace network
