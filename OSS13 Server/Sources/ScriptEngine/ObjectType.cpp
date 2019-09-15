#include "ObjectType.h"

#include <plog/Log.h>

ObjectType::ObjectType(py::handle cls) : 
	cls(cls)
{
	name = py::str(cls.attr("__name__"));
	typeKey = std::string(py::str(cls.attr("__module__"))) + "." + name;
	module = py::module::import("inspect").attr("getmodule")(cls);
}

const std::string &ObjectType::GetName() { return name; }
const std::string &ObjectType::GetTypeKey() { return typeKey; }
py::handle ObjectType::GetHandle() { return cls; }
py::handle ObjectType::GetModule() { return module; }
