#pragma once

#include <Shared/Network/Archive.h>
#include <Shared/Network/Syncable.h>

namespace network {
namespace sync {

DEFINE_SYNCABLE(ObjectSyncFields)
protected:
	uf::SyncField<std::string> name;

public:
	ObjectSyncFields() { 
		DECLARE_SYNCABLE_FIELD(&ObjectSyncFields::name); 
	};
DEFINE_SYNCABLE_END 

} // namespace sync
} // namespace network
