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
    friend Tile;

    Object();
    virtual ~Object() = default;

    virtual void Update(sf::Time timeElapsed);

    virtual void Interact(Object *) = 0;
    void AddComponent(Component *);
    void SetConstSpeed(uf::vec2f speed);
    void SetSprite(const std::string &sprite);
    void PlayAnimation(const std::string &sprite);
    void Delete();

    uint ID() const;
    std::string GetName() const;
    Tile *GetTile() const;
    template<class T> T *GetComponent();

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

    const ObjectInfo GetObjectInfo() const;

protected:
    std::string name;
    bool density;
    bool movable;
    std::string sprite;
    // Object layer 0-100. The smaller layer is lower.
    uint layer;
    uf::Direction direction;

    // Invisibility
    //// 8 bits for different kinds of invisibility
    //// Last bit - ghost invisibility
    uint invisibility;
    //

private:
    uint id;
    Tile *tile;
    std::list<uptr<Component>> components;

    // Movement
    float moveSpeed;
    uf::vec2i moveIntent;
    uf::vec2f constSpeed;
    uf::vec2f physSpeed;

    uf::vec2f shift;
};

template <class T> T *Object::GetComponent() {
	for (auto &component : components) {
		if (auto temp = dynamic_cast<T *>(component.get()))
			return temp;
	}
	return nullptr;
}
