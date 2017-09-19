#pragma once

#include <SFML/System/Time.hpp>

class Object;

class Component {
protected:
	Object *owner;

public:
    Component();
    virtual ~Component() = default;

	virtual void Update(sf::Time timeElapsed) = 0;

	Object *GetOwner() const;
	void SetOwner(Object *owner);
};

