#pragma once

#include <string>
#include <list>

#include "Shared/Types.hpp"
#include "Shared/Global.hpp"

#include "Component.hpp"

class Tile;
struct ObjectInfo;

class Object {
public:
    Object();
    virtual ~Object() = default;

    virtual void Update(sf::Time timeElapsed);

    virtual void Interact(Object *) = 0;

    void AddComponent(Component *);
    template <class T> T *GetComponent();

    uint ID() const;
    std::string GetName() const;
    Tile *GetTile() const;

    bool GetDensity() const;
    bool IsMovable() const;
    // True if visibility bits allows to see invisibility bits
    bool CheckVisibility(uint visibility) const;
    // Get Invisibility bits
    uint GetInvisibility() const;

	//Global::Sprite GetSprite() const;
    uint GetLayer() const;

	//
	// For control purposes
	//
		uf::vec2f GetShift() const;
		//float GetSpeed() const;

		void SetDirection(uf::Direction);
		void AddShift(uf::vec2f);
		void SetShift(uf::vec2f);
	//

    const ObjectInfo GetObjectInfo() const;

    friend Tile;

protected:
    std::string name;
    bool density;
    bool movable;
    std::string sprite;
    uint layer;
    uf::Direction direction;

    // Invisibility
    //// 8 bits for different kinds of invisibility
    //// Last bit - ghost invisibility
    uint invisibility;
    //

    uf::vec2f shift;
    //float speed; // speed (tiles/seconds)

private:
    uint id;
    Tile *tile;
    std::list<uptr<Component>> components;

    void takeID();
};

template <class T> T *Object::GetComponent() {
	for (auto &component : components) {
		if (auto temp = dynamic_cast<T *>(component.get()))
			return temp;
	}
	return nullptr;
}