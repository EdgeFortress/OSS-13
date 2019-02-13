#pragma once

#include <string>
#include <list>
#include <vector>

#include <World/Objects/Component.hpp>
#include <Resources/IconInfo.h>

#include <Shared/Types.hpp>
#include <Shared/Global.hpp>
#include <Shared/Timer.h>

class ObjectHolder;
class Tile;
struct ObjectInfo;

class Object {
	friend ObjectHolder;
	friend Tile;

protected:
	Object(); // Use ObjectHolder to create objects!

public:
    virtual ~Object() = default;

	virtual void AfterCreation();
    virtual void Update(sf::Time timeElapsed);

    virtual bool InteractedBy(Object *) = 0;

	void AddObject(Object *);
	virtual bool RemoveObject(Object *);
    void AddComponent(Component *);
    void SetConstSpeed(uf::vec2f speed);
    void SetSprite(const std::string &sprite);
	void SetSpriteState(Global::ItemSpriteState);
	// False if another animation is playing already. Callback will be called after animation
	bool PlayAnimation(const std::string &sprite, std::function<void()> &&callback = {});
    void Delete();

    uint ID() const;
    std::string GetName() const;
    Tile *GetTile() const;
	Object *GetHolder() const;
    template<class T> T *GetComponent();

    bool GetDensity() const;
    bool IsMovable() const;
    bool IsCloseTo(Object *) const;
    // True if visibility bits allows to see invisibility bits
    bool CheckVisibility(uint visibility) const;
    // Get Invisibility bits
    uint GetInvisibility() const;

	std::string GetSprite() const;

    uint GetLayer() const;

	//
	// For control purposes
	//
		//uf::vec2f GetShift() const;
		void SetDirection(uf::Direction);

        void SetMoveIntent(uf::vec2i);
        uf::vec2i GetMoveIntent() const;

        void SetMoveSpeed(float speed);
        float GetMoveSpeed() const;
        //void SetSpeed(float speed);
		//void AddShift(uf::vec2f);
		//void SetShift(uf::vec2f);
	//

	ObjectInfo GetObjectInfo() const;

protected:
	// refresh std::vector<uint32_t> icons
	// last in, last drawn
	// shouldn't be called as is, use askToUpdateIcons()!
	virtual void updateIcons() const;
	void askToUpdateIcons();

private:
	// for use from Tile
	void setTile(Tile *);

protected:
    std::string name;
    bool density;
    bool movable;
    std::string sprite;
	Global::ItemSpriteState spriteState; // TODO: move it to Item? Also there is need to reimplement packing???
	uf::Timer animationTimer;
    // Object layer 0-100. The smaller layer is lower.
    uint layer;
    uf::Direction direction;

    // Invisibility
    //// 8 bits for different kinds of invisibility
    //// Last bit - ghost invisibility
    uint invisibility;
    //

	mutable std::vector<IconInfo> icons;

private:
    uint id;
    Tile *tile;
	Object *holder;
	std::list<Object *> content;
    std::list<uptr<Component>> components;

    // Movement
    float moveSpeed;
    uf::vec2i moveIntent;
    uf::vec2f constSpeed;
    uf::vec2f physSpeed;

    uf::vec2f shift;

	bool iconsOutdated;
};

template <class T> T *Object::GetComponent() {
	for (auto &component : components) {
		if (auto temp = dynamic_cast<T *>(component.get()))
			return temp;
	}
	return nullptr;
}
