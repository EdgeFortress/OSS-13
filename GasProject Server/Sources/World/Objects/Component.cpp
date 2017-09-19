#include "Component.hpp"

Component::Component() :
    owner(nullptr)
{ }

Object *Component::GetOwner() const { return owner; }
void Component::SetOwner(Object *owner) { this->owner = owner; }

