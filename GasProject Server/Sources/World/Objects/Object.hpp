#pragma once

#include <string>
#include <list>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Shared/Types.hpp"
#include "Shared/Geometry.hpp"
#include "Shared/Global.hpp"

#include "Component.hpp"

class Tile;
struct ObjectInfo;

namespace sf {
    class Packet;
}

class Object {
private:
	uf::Direction direction;
    void takeID();

	std::list<uptr<Component>> components;

protected:
    uint id;
	std::string name;

    Tile *tile;
	bool density;
    
    Global::Sprite sprite;
    uint layer;

	// Invisibility
	//// 8 bits for different kinds of invisibility
	//// Last bit - ghost invisibility
		uint invisibility;
	//
    
	sf::Vector2f shift;
	//float speed; // speed (tiles/seconds)

public:
    Object();

	virtual void Interact(Object *) = 0;

    virtual void Update(sf::Time timeElapsed);

	void AddComponent(Component *);
	template <class T> T *GetComponent();

	bool GetDensity() const;
	Global::Sprite GetSprite() const;
	bool CheckVisibility(uint visibility) const;
	uint GetInvisibility() const;
	Tile *GetTile() const;
	std::string GetName() const;
	uint ID() const;

	//
	// For control purposes
	//
		sf::Vector2f GetShift() const;
		float GetSpeed() const;

		void SetDirection(uf::Direction);
		void AddShift(sf::Vector2f);
		void SetShift(sf::Vector2f);
	//

    //// Just set tile pointer
    //void SetTile(Tile *tile) { this->tile = tile; }

    const ObjectInfo GetObjectInfo() const;

    friend Tile;
    //friend sf::Packet &operator<<(sf::Packet &, const Object &);
};

template <class T> T *Object::GetComponent() {
	for (auto &component : components) {
		if (auto temp = dynamic_cast<T *>(component.get()))
			return temp;
	}
	return nullptr;
}