#pragma once

#include <string>
#include <list>
#include <vector>
#include <chrono>

#include <VerbsHolder.h>
#include <World/Objects/Component.hpp>
#include <Resources/IconInfo.h>

#include <Shared/Types.hpp>
#include <Shared/Global.hpp>
#include <Shared/Timer.h>
#include <Shared/IFaces/INonCopyable.h>
#include <Shared/Geometry/DirectionSet.h>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

class ObjectHolder;
class Tile;

class Object : public VerbsHolder, public INonCopyable {
	friend ObjectHolder;
	friend Tile;

public:
	Object(); // Use ObjectHolder to create objects!
	virtual ~Object() = default;

    virtual void Update(std::chrono::microseconds timeElapsed);

	virtual bool InteractedBy(Object *) = 0;

	virtual void Move(uf::vec2i order);
	virtual void MoveZ(int order) {};

	void AddComponent(Component *);
	void AddComponent(const std::string &componentId);
	Component *GetComponent(const std::string &componentId);
	template<class T> 
	T *GetComponent();

	void AddObject(Object *);
	virtual bool RemoveObject(Object *);
    void SetConstSpeed(uf::vec2f speed);
    virtual void Delete();

    uint ID() const;

    const std::string &GetName() const;
	void SetName(const std::string& name);

	const std::string &GetSprite() const;
	void SetSprite(const std::string &sprite);

	uint GetLayer() const;
	void SetLayer(uint);

	void SetSpriteState(Global::ItemSpriteState);
	// False if another animation is playing already. Callback will be called after animation
	bool PlayAnimation(const std::string &sprite, std::function<void()> callback = {});

	void SetDensity(bool);
	bool GetDensity() const;

	void SetSolidity(uf::DirectionSet directions);
	uf::DirectionSet GetSolidity() const;

	void SetOpacity(uf::DirectionSetFractional fractionalDirections);
	uf::DirectionSetFractional GetOpacity() const;

	void SetAirtightness(uf::DirectionSetFractional fractionalDirections);
	uf::DirectionSetFractional GetAirtightness() const;

	void SetPosition(uf::vec2i);
	uf::vec2i GetPosition() const;

	Tile *GetTile() const;
	void SetTile(Tile *tile);

	Object *GetHolder() const;
	virtual sptr<Object> GetOwnershipPointer() = 0;
	bool CheckIfJustCreated() { return justCreated ? justCreated = false, true : false; }; // TODO: remove this
	bool CheckIfMarkedToBeDeleted() { return markedToBeDeleted; }
	int IncreaseDeleteAttempts() { return ++deleteAttempts; }

	bool IsMovable() const;
	bool IsCloseTo(Object *) const;
	// True if visibility bits allow to see invisibility bits
	bool CheckVisibility(uint viewerId, uint visibility) const;
	void SetInvisibility(uint);
	uint GetInvisibility() const;

	//
	// For control purposes
	//
		//uf::vec2f GetShift() const;
		void SetDirection(uf::Direction);

        void SetMoveIntent(uf::vec2i);
        uf::vec2i GetMoveIntent() const;

        void SetMoveSpeed(float speed);
        float GetMoveSpeed() const;
        float GetSpeed() const;
		//void AddShift(uf::vec2f);
		//void SetShift(uf::vec2f);
	//

	bool IsFloor() const;
	void SetIsFloor(bool value);

	bool IsWall() const;
	void SetIsWall(bool value);

	network::protocol::ObjectInfo GetObjectInfo() const;

	// refresh std::vector<uint32_t> icons
	// last in, last drawn
	// shouldn't be called as is, use askToUpdateIcons()!
	virtual void updateIcons();
	void pushToIcons(const IconInfo &icon);
	void askToUpdateIcons();

private:
	// for use from Tile
	void setTile(Tile *);

protected:
    std::string name;
    bool movable;
    std::string sprite;
	Global::ItemSpriteState spriteState; // TODO: move it to Item? Also there is need to reimplement packing???
	uf::Timer animationTimer;
    // Object layer 0-100. The smaller layer is lower.
    uint layer;
    uf::Direction direction;

	uf::DirectionSet solidity;
	uf::DirectionSetFractional opacity;
	uf::DirectionSetFractional airtightness;

    // Invisibility
    //// 8 bits for different kinds of invisibility
    //// Last bit - ghost invisibility
	uint invisibility{0};
    //

	mutable std::vector<IconInfo> icons;

private:
	uint id;
    Tile *tile;
	Object *holder;
	std::list<Object *> content;
	std::unordered_map<std::string, uptr<Component>> components;

    // Movement
    float moveSpeed;
    uf::vec2i moveIntent;
    uf::vec2f constSpeed;
    uf::vec2f physSpeed;

	// atmos
	bool isFloor{false};
	bool isWall{false};

    uf::vec2f shift;

	bool justCreated{true};
	bool markedToBeDeleted{false};
	int deleteAttempts{0};
	bool iconsOutdated{true};
};

template <class T> T *Object::GetComponent() {
	for (auto &idAndComponent : components) {
		if (auto temp = dynamic_cast<T *>(idAndComponent.second.get()))
			return temp;
	}
	return nullptr;
}
