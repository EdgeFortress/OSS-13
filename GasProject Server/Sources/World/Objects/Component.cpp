#include "Component.hpp"

Object *Component::GetOwner() const { return owner; }
void Component::SetOwner(Object *owner) { this->owner = owner; }

