#include "Syncable.h"

namespace uf {

namespace detail {

void GeneralSyncField::setChanged() { changed = true; syncable->changed = true; }


} // namespace detail

SyncableChanges Syncable::PopChanges() {
	SyncableChanges changes;
	changes.archive.SetMode(Archive::Mode::Input);
	std::size_t changedCount = 0;
	for (std::size_t i = 0; i < fieldsOffsets.size(); i++) {
		if (getField(i)->changed)
			changedCount++;
	}
	changes.archive << sf::Int32(changedCount);

	for (std::size_t i = 0; i < fieldsOffsets.size(); i++) {
		if (getField(i)->changed) {
			changes.archive << sf::Int32(i);
			getField(i)->Serialize(changes.archive);
			getField(i)->changed = false;
		}
	}
	changed = false;
	return changes;
}

void Syncable::AmendChanges(SyncableChanges &&changes) {
	changes.archive.SetMode(Archive::Mode::Output);
	sf::Int32 changedCount;
	changes.archive >> changedCount;
	while (changedCount--) {
		sf::Int32 fieldNumber;
		changes.archive >> fieldNumber;
		changes.archive >> *getField(std::size_t(fieldNumber));
	};
}

void Syncable::ResetChanges() {
	for (std::size_t i = 0; i < fieldsOffsets.size(); i++)
		getField(i)->changed = false;
	changed = false;
}

bool Syncable::IsChanged() {
	return changed;
}

void Syncable::Serialize(Archive &ar) {
	uf::ISerializable::Serialize(ar);
	for (std::size_t i = 0; i < fieldsOffsets.size(); i++) {
		ar & *getField(i);
	}
}

void Syncable::registrateField(std::ptrdiff_t fieldOffset) {
	fieldsOffsets.push_back(fieldOffset);
	getField(fieldsOffsets.size() - 1)->syncable = this;
}

detail::GeneralSyncField *Syncable::getField(size_t index) {
	return reinterpret_cast<detail::GeneralSyncField *>(size_t(this) + fieldsOffsets[index]);
}

} // namespace uf
