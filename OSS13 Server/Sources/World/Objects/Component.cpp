#include "Component.hpp"

Component::Component(std::string &&id) :
	id(std::forward<std::string>(id))
{ }

const std::string &Component::ID() { return id; }
Object *Component::GetOwner() const { return owner; }
void Component::SetOwner(Object *owner) { this->owner = owner; }

