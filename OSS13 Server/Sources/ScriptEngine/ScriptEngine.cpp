#include "ScriptEngine.h"

#include <Python.h>

#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <plog/Log.h>

#include <Player.hpp>
#include <World/Map.hpp>
#include <World/Objects/Object.hpp>

#include <Shared/Global.hpp>
#include <Shared/ErrorHandling.h>

namespace py = pybind11;

ScriptEngine::ScriptEngine() {
	py::initialize_interpreter();
	py::module::import("sys").attr("path").attr("append")("GameLogic");
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

void ScriptEngine::FillMap(Map *map) {
	try {
		py::module::import("Map").attr("FillMap")(map);
	} catch (const std::exception &e) {
		MANAGE_EXCEPTION(e);
	}
}

void ScriptEngine::OnPlayerJoined(Player *player) {
	try {
		py::module::import("Engine.HooksRawArgs").attr("rawOnPlayerJoined")(player);
	} catch (const std::exception &e) {
		MANAGE_EXCEPTION(e);
	}
}
