#pragma once

#include <string>
#include <chrono>

class Object;

class Component {
	std::string id;

protected:
	Object *owner{};

public:
    Component(std::string &&id);
    virtual ~Component() = default;

	virtual void Update(std::chrono::microseconds timeElapsed) = 0;

	const std::string &ID();
	Object *GetOwner() const;
	virtual void SetOwner(Object *owner);
};
