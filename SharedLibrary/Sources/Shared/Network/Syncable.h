#pragma once

#include <vector>

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>

namespace uf {

struct Syncable;

namespace detail {

template<class T, typename U>
std::ptrdiff_t memberOffset(U T::* member)
{
	return reinterpret_cast<std::ptrdiff_t>(
		&(reinterpret_cast<T const volatile*>(NULL)->*member)
	);
}

DEFINE_SERIALIZABLE(GeneralSyncField, uf::ISerializable)
	friend Syncable;
public:
	GeneralSyncField() = default;
	~GeneralSyncField() override = default;
	GeneralSyncField(const GeneralSyncField &other) : changed(other.changed) { }
	GeneralSyncField &operator=(const GeneralSyncField &other) { this->changed = other.changed; return *this; }
	GeneralSyncField(GeneralSyncField &&other) : changed(std::move(other.changed)) { }
	GeneralSyncField &operator=(GeneralSyncField &&other) { this->changed = std::move(other.changed); return *this; }

protected:
	void setChanged();

private:
	bool changed{false};
	Syncable *syncable{};
DEFINE_SERIALIZABLE_END

} // namespace detail

template <typename T>
struct SyncField : public detail::GeneralSyncField {
	friend Syncable;
public:
	SyncField &operator=(const T &value) { this->value = value; setChanged(); return *this; }
	operator const T &() const { return value; }

	const T &GetValue() { return value; };

	void Serialize(Archive &ar) final {
		uf::ISerializable::Serialize(ar);
		ar & value;
		if (ar.GetMode() == uf::Archive::Mode::Output)
			setChanged();
	}

private:
	T value{};
};

DEFINE_SERIALIZABLE(SyncableChanges, uf::ISerializable)
	friend Syncable;

	void Serialize(Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & archive;
	}

protected:
	uf::Archive archive;
DEFINE_SERIALIZABLE_END

struct Syncable : public uf::ISerializable {
	friend detail::GeneralSyncField;

	SyncableChanges PopChanges();
	void AmendChanges(SyncableChanges &&changes);
	void ResetChanges();

	bool IsChanged();

	void Serialize(Archive &ar) final;

protected:
	void registrateField(std::ptrdiff_t fieldOffset);

private:
	detail::GeneralSyncField *getField(size_t index);

private:
	bool changed{false};
	std::vector<std::ptrdiff_t> fieldsOffsets;
};

} // namespace uf

/*
	Start of Syncable structure definition

	Declare all fields in Syncable constructor with DECLARE_SYNCABLE_FIELD macro.
*/
#define DEFINE_SYNCABLE(TypeName)                                             \
	DEFINE_SERIALIZABLE(TypeName, uf::Syncable)                               \
		TypeName(const TypeName &other) { TypeName(); *this = other; };       \
		TypeName &operator=(const TypeName &other) = default;                 \
		TypeName(TypeName &&other) { TypeName(); *this = std::move(other); }; \
		TypeName &operator=(TypeName &&other) = default;                      \

/*
	End of Syncable structure definition
*/
#define DEFINE_SYNCABLE_END \
	DEFINE_SERIALIZABLE_END

#define DECLARE_SYNCABLE_FIELD(field) \
	registrateField(uf::detail::memberOffset(field));
