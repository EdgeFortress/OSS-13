#include "ObjectType.h"

#include <plog/Log.h>

#include <Server.hpp>

ObjectType::ObjectType(py::handle cls) : 
	cls(cls)
{
	typeName = py::str(cls.attr("__name__"));
	typeKey = std::string(py::str(cls.attr("__module__"))) + "." + typeName;
	module = py::module::import("inspect").attr("getmodule")(cls);

	canBeSpawned = py::bool_(cls.attr("canBeSpawned"));

	auto name = cls.attr("defName");
	if (!name.is_none())
		this->name = py::str(name);
	else
		canBeCreatedByEngine = false;

	auto sprite = cls.attr("defSprite");
	if (!sprite.is_none()) {
		std::string spriteKey = py::str(sprite);
		if (!spriteKey.empty())
			this->sprite = GServer->RM()->GetIconInfo(spriteKey).id;
	}

	auto description = cls.attr("defDescription");
	if (!description.is_none()) {
		this->description = py::str(description);
	}
}

bool ObjectType::CanBeCreatedByEngine() const { return canBeCreatedByEngine; }
bool ObjectType::CanBeSpawned() const { return CanBeCreatedByEngine() && canBeSpawned; }
const std::string &ObjectType::GetName() const { return typeName; }
const std::string &ObjectType::GetTypeKey() const { return typeKey; }
py::handle ObjectType::GetHandle() { return cls; }
py::handle ObjectType::GetModule() { return module; }
