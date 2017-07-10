#pragma once

#include <string>
#include <list>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Common/NetworkConst.hpp"
#include "Common/Useful.hpp"

#include "Component.hpp"

class Tile;
struct ObjectInfo;

namespace sf {
    class Packet;
}

class Object {
private:
	Global::Direction direction;
    void takeID();

	std::list<uptr<Component>> components;

protected:
    uint id;
	std::string name;

    Tile *tile;
	bool density;
    
    Global::Sprite sprite;
    uint layer;
    
	sf::Vector2f shift;
	//float speed; // speed (tiles/seconds)

public:
    Object();

	virtual void Interact(Object *) = 0;

    virtual void Update(sf::Time timeElapsed);

	void AddComponent(Component *);
	template <class T> T *GetComponent();

    bool GetDensity() const { return density; }
    Global::Sprite GetSprite() const { return sprite; }
    Tile *GetTile() const { return tile; }
    std::string GetName() const { return name; }
    uint ID() const { return id; }

	//
	// For control purposes
	//
		sf::Vector2f GetShift() const;
		float GetSpeed() const;

		void SetDirection(Global::Direction);
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