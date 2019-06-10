#include "ScriptEngine.h"

#include <Python.h>

#include <pybind11/embed.h>
#include <plog/Log.h>

#include <Shared/ErrorHandling.h>

#include <IGame.h>
#include <World/World.hpp>

#include "Trampoline/PyObject.h"

namespace py = pybind11;
namespace se = script_engine;

PYBIND11_EMBEDDED_MODULE(Engine, m) {
	py::class_<Object, se::PyObject, PyObjectPtr<Object>> object(m, "Object");
	object
		.def(py::init<>())
		.def("Update", &Object::UpdateWithoutTime)
		.def_property("name", &Object::GetName, &Object::SetName)
		.def_property("sprite", &Object::GetSprite, &Object::SetSprite)
		.def_property("layer", &Object::GetLayer, &Object::SetLayer)
		.def_property("density", &Object::GetDensity, &Object::SetDensity);
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
		LOGE << "Failed to create script object" << "\n"
			 << e.what();
		return nullptr;
	}
}
