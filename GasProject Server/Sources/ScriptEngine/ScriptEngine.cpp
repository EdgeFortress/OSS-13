#include "ScriptEngine.h"

#include <Python.h>

#include <pybind11/embed.h>
#include <plog/Log.h>

#include <Shared/ErrorHandling.h>

#include "Trampoline/PyObject.h"
#include "Trampoline/PyComponent.h"

#include <World/Objects/Control.hpp>

#include <Shared/Geometry/Vec2.hpp>

namespace py = pybind11;
namespace se = script_engine;

PYBIND11_EMBEDDED_MODULE(Engine, m) {
	py::class_<uf::vec2i>(m, "Vec2i")
		.def(py::init<>())
		.def(py::init<int, int>())
		.def(py::init<int>())
		.def_readwrite("x", &uf::vec2i::x)
		.def_readwrite("y", &uf::vec2i::y)
		.def("__repr__", &uf::vec2i::toString)
		.def("__bool__", &uf::vec2i::operator bool);

	py::class_<Object, se::PyObject, PyObjectPtr<Object>>(m, "Object")
		.def(py::init<>())
		.def_property("name", &Object::GetName, &Object::SetName)
		.def_property("sprite", &Object::GetSprite, &Object::SetSprite)
		.def_property("layer", &Object::GetLayer, &Object::SetLayer)
		.def_property("density", &Object::GetDensity, &Object::SetDensity)
		.def_property("invisibility", &Object::GetInvisibility, &Object::SetInvisibility)
		.def("Update", &Object::Update)
		.def("Move", &Object::Move)
		.def("MoveZ", &Object::MoveZ)
		.def("AddComponent", (void (Object::*)(const std::string &)) &Object::AddComponent)
		.def("GetComponent", (Component *(Object::*)(const std::string &)) &Object::GetComponent);

	py::class_<Component, se::PyComponent>(m, "Component")
		.def(py::init<std::string &&>())
		.def("Update", &Component::Update);

	py::class_<Control, Component>(m, "Control")
		.def("GetAndDropMoveOrder", &Control::GetAndDropMoveOrder)
		.def("GetAndDropMoveZOrder", &Control::GetAndDropMoveZOrder);
}

ScriptEngine::ScriptEngine() {
	py::initialize_interpreter();
	py::module::import("sys").attr("path").attr("append")("Script");
	py::print("Script Engine: start.");
}

ScriptEngine::~ScriptEngine() {
	py::print("Script Engine: stop.");
	py::finalize_interpreter();
}

Object *ScriptEngine::CreateObject(const std::string& m, const std::string& type) {
	try {
		if (!type.size()) {
			auto index = m.find_last_of('.');
			EXPECT_WITH_MSG(index != std::string::npos, "Wrong script object module! Module is " + m);
			std::string type = m.substr(m.find_last_of('.') + 1);
			return py::module::import(m.c_str()).attr(type.c_str())().cast<Object *>();
		} else {
			return py::module::import(m.c_str()).attr(type.c_str())().cast<Object *>();
		}
	} catch (const std::exception &e) {
		LOGE << "Failed to create script object " << m << " " << type << "\n"
			 << e.what();
		return nullptr;
	}
}
