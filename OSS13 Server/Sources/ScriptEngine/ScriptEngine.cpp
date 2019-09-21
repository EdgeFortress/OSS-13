#include "ScriptEngine.h"

#include <cctype>
#include <algorithm>
#include <filesystem>

#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <plog/Log.h>

#include <Player.hpp>
#include <World/Map.hpp>
#include <World/Objects/Object.hpp>

#include <Shared/Global.hpp>
#include <Shared/ErrorHandling.h>
#include <Shared/OS.hpp>

using namespace std::string_literals;

void findSubstringsAndReplaceAll(std::string &str, const std::string &toSearch, const std::string &replaceStr) {
	size_t pos = str.find(toSearch);

	while (pos != std::string::npos) {
		str.replace(pos, toSearch.size(), replaceStr);
		pos = str.find(toSearch);
	}
}

void importAllModules() {
	auto scripts = FindFilesRecursive(std::filesystem::path("GameLogic"), "*.py");
	for (auto &script : scripts) {
		try {
			std::string module = std::filesystem::relative(script.string(), "GameLogic").string();
			findSubstringsAndReplaceAll(module, ".py", "");
			findSubstringsAndReplaceAll(module, "\\\\", ".");
			findSubstringsAndReplaceAll(module, "\\", ".");
			findSubstringsAndReplaceAll(module, "/", ".");
			py::module::import(module.c_str());
			LOGI << "\tModule loaded: "s + module;
		} catch (const std::exception &e) {
			MANAGE_EXCEPTION_WITH_MSG(e, "Failed to load \""s + script.string() + "\" module!");
		}
	}
}

void obtainObjectTypes(std::map<ObjectTypeId, std::unique_ptr<ObjectType>> &objectTypes) {
	try {
		auto objectClass = py::module::import("Engine.World").attr("Object");
		auto classes = py::set(py::module::import("Engine.Detail.Environment").attr("GetInheritors")(objectClass));

		for (auto cls : classes) {
			auto objectType = std::make_unique<ObjectType>(cls);
			objectTypes.insert({objectType->GetTypeKey(), std::move(objectType)});
		}

		for (auto &[key, type]: objectTypes)
			LOGI << "\tType registered: "s + key;
	} catch (const std::exception &e) {
		MANAGE_EXCEPTION_WITH_MSG(e, "Failed to obtain the list of Object types!");
	}
}

ScriptEngine::ScriptEngine() {
	py::initialize_interpreter();
	py::module::import("sys").attr("path").attr("append")("GameLogic");

	LOGI << "Script Engine: start loading modules.";
	importAllModules();

	LOGI << "Script Engine: obtain objects types.";
	obtainObjectTypes(objectTypes);

	LOGI << "Script Engine: successfully initialized.";
}

ScriptEngine::~ScriptEngine() {
	LOGI << "Script Engine: is stopping.";
	py::finalize_interpreter();
}

Object *ScriptEngine::CreateObject(const std::string& typeKey) {
	try {
		return CreateObject(*objectTypes.at(typeKey));
	} catch (const std::exception &e) {
		MANAGE_EXCEPTION_WITH_MSG(e, "Failed to create script object (TypeKey: \"" + typeKey + "\")\n");
		return nullptr;
	}
}

std::vector<network::protocol::ObjectType *> GetSelectedTypesInfo(
	const std::map<ObjectTypeId, std::unique_ptr<ObjectType>> &objectTypes, 
	const std::function<bool(const ObjectType &type)> &selector) 
{
	std::vector<network::protocol::ObjectType *> result;

	for (auto &[key, type] : objectTypes) {
		if (selector(*type) && type->CanBeSpawned())
			result.push_back(type.get());
	}

	return result;
}

bool isStringIncludeCaseInsensitive(const std::string &string, const std::string &pattern)
{
	auto it = std::search(
		string.begin(), string.end(),
		pattern.begin(), pattern.end(),
		[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
	);
	return (it != string.end());
}

std::vector<network::protocol::ObjectType *> ScriptEngine::GetTypesInfo(const std::string &searchString) {
	std::function<bool(const ObjectType &type)> selector;

	if (searchString.empty()) {
		selector = [&searchString](const ObjectType &type) { return true; };
	} else {
		selector = [&searchString](const ObjectType &type) {
			if (isStringIncludeCaseInsensitive(type.GetTypeKey(), searchString))
				return true;
			return false;
		};
	}

	return GetSelectedTypesInfo(objectTypes, selector);
}

void ScriptEngine::FillMap(Map *map) {
	try {
		py::module::import("Engine.Detail.HooksRawArgs").attr("rawFillMap")(map);
	} catch (const std::exception &e) {
		MANAGE_EXCEPTION(e);
	}
}

void ScriptEngine::OnPlayerJoined(Player *player) {
	try {
		py::module::import("Engine.Detail.HooksRawArgs").attr("rawOnPlayerJoined")(player);
	} catch (const std::exception &e) {
		MANAGE_EXCEPTION(e);
	}
}

ObjectType &ScriptEngine::GetObjectType(const std::string& module, const std::string& type) {
	if (type.size()) {
		return *objectTypes[module + "." + type];
	} else {
		auto index = module.find_last_of('.');
		EXPECT_WITH_MSG(index != std::string::npos, "Bad module path! Module: \"" + module + "\"");
		std::string type = module.substr(module.find_last_of('.') + 1);
		return *objectTypes[module + "." + type];
	}
}

Object *ScriptEngine::CreateObject(ObjectType &type) {
	if (!type.CanBeCreatedByEngine()) {
		LOGE << type.GetTypeKey() << " type can't be created! Check whether name is None.";
		return nullptr;
	}
	return type.GetHandle()().cast<Object *>();
}
