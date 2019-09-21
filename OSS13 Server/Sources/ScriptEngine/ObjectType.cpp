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
	name = py::str(cls.attr("defName"));
	std::string spriteKey = py::str(cls.attr("defSprite"));
	if (spriteKey.empty())
		sprite = 0;
	else
		sprite = GServer->RM()->GetIconInfo(spriteKey).id;
	description = py::str(cls.attr("defDescription"));
}

bool ObjectType::CanBeSpawned() const { return canBeSpawned; }
const std::string &ObjectType::GetName() const { return typeName; }
const std::string &ObjectType::GetTypeKey() const { return typeKey; }
py::handle ObjectType::GetHandle() { return cls; }
py::handle ObjectType::GetModule() { return module; }
