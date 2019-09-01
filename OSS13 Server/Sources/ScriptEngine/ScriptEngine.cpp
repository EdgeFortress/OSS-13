#include "ScriptEngine.h"

#include <Python.h>

#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <plog/Log.h>

#include <Shared/ErrorHandling.h>

#include "Trampoline/PyObject.h"
#include "Trampoline/PyComponent.h"

#include <IServer.h>
#include <Game.h>
#include <VerbsHolder.h>
#include <Player.hpp>
#include <Resources/ResourceManager.hpp>
#include <World/Tile.hpp>
#include <World/Map.hpp>
#include <World/Objects/CreateObject.h>
#include <World/Objects/Control.hpp>
#include <World/Objects/ControlUI.h>

#include <Shared/Global.hpp>
#include <Shared/Geometry/Vec2.hpp>

namespace py = pybind11;
namespace se = script_engine;

template<typename T>
void RegistratePlaneVector(pybind11::module m, const char *name) {
	py::class_<uf::vec2<T>>(m, name)
		.def(py::init<>())
		.def(py::init<T, T>())
		.def(py::init<T>())
		.def_readwrite("x", &uf::vec2<T>::x)
		.def_readwrite("y", &uf::vec2<T>::y)
		.def("Normalize", &uf::vec2<T>::normalize)
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self * int())
		.def(py::self * float())
		.def(py::self / int())
		.def(py::self / float())
		.def("__repr__", &uf::vec2<T>::toString)
		.def("__bool__", &uf::vec2<T>::operator bool);
}

template<typename T>
void RegistrateSpaceVector(pybind11::module m, const char *name) {
	py::class_<uf::vec3<T>>(m, name)
		.def(py::init<>())
		.def(py::init<T, T, T>())
		.def(py::init<T>())
		.def_readwrite("x", &uf::vec3<T>::x)
		.def_readwrite("y", &uf::vec3<T>::y)
		.def_readwrite("z", &uf::vec3<T>::z)
		.def("Normalize", &uf::vec3<T>::normalize)
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self * int())
		.def(py::self * float())
		.def(py::self / int())
		.def(py::self / float())
		.def("__repr__", &uf::vec3<T>::toString)
		.def("__bool__", &uf::vec3<T>::operator bool);
}

PYBIND11_EMBEDDED_MODULE(Engine, m) {
	RegistratePlaneVector<int32_t> (m, "Vec2i");
	RegistratePlaneVector<double>  (m, "Vec2f");
	RegistrateSpaceVector<int32_t>(m, "Vec3i");
	RegistrateSpaceVector<double>(m, "Vec3f");

	//RegistratePlaneVector<int32_t>(m, "Vec2i_32"); // only for compatibility, _32 types are useless in python
	RegistratePlaneVector<uint32_t>(m , "Vec2i_unsigned");
	RegistratePlaneVector<float>(m, "Vec2f_32");

	py::enum_<uf::Direction>(m, "Direction")
		.value("NONE",       uf::Direction::NONE)
		.value("SOUTH",      uf::Direction::SOUTH)
		.value("WEST",       uf::Direction::WEST)
		.value("NORTH",      uf::Direction::NORTH)
		.value("EAST",       uf::Direction::EAST)
		.value("SOUTH_WEST", uf::Direction::SOUTH_WEST)
		.value("NORTH_WEST", uf::Direction::NORTH_WEST)
		.value("NORTH_EAST", uf::Direction::NORTH_EAST)
		.value("SOUTH_EAST", uf::Direction::SOUTH_EAST)
		.value("CENTER",     uf::Direction::CENTER);

	py::class_<uf::DirectionSet>(m, "DirectionSet")
		.def(py::init<>())
		.def("Add", (void (uf::DirectionSet::*)(const std::list<Direction> &)) &uf::DirectionSet::Add)
		.def("Remove", (void (uf::DirectionSet::*)(const std::list<Direction> &)) &uf::DirectionSet::Remove)
		.def("IsExistsOne", (bool (uf::DirectionSet::*)(const std::list<Direction> &) const) &uf::DirectionSet::IsExistsOne)
		.def("AreExistAll", (bool (uf::DirectionSet::*)(const std::list<Direction> &) const) &uf::DirectionSet::AreExistAll)
		.def("Reset", &uf::DirectionSet::Reset);

	py::class_<uf::DirectionSetFractional>(m, "DirectionSetFractional")
		.def("Add", &uf::DirectionSetFractional::Add);

	py::class_<VerbsHolder>(m, "VerbHolder")
		.def("AddVerb", &VerbsHolder::AddVerb);

	py::class_<Player, VerbsHolder>(m, "Player")
		.def_property_readonly("ckey", &Player::GetCKey)
		.def_property("control", &Player::GetControl, &Player::SetControl, "", py::return_value_policy::reference);

	py::class_<Tile>(m, "Tile")
		.def_property_readonly("x", &Tile::X)
		.def_property_readonly("y", &Tile::Y)
		.def_property_readonly("z", &Tile::Z)
		.def("IsDense", py::overload_cast<>(&Tile::IsDense, py::const_))
		.def("IsSpace", &Tile::IsSpace)
		.def("GetDenseObject", &Tile::GetDenseObject, py::return_value_policy::reference);

	py::class_<Object, se::PyObject, PyObjectPtr<Object>>(m, "Object")
		.def(py::init<>())
		.def_property("name", &Object::GetName, &Object::SetName)
		.def_property("sprite", &Object::GetSprite, &Object::SetSprite)
		.def_property("layer", &Object::GetLayer, &Object::SetLayer)
		.def_property("density", &Object::GetDensity, &Object::SetDensity)
		.def_property("solidity", &Object::GetSolidity, &Object::SetSolidity)
		.def_property("opacity", &Object::GetOpacity, &Object::SetOpacity)
		.def_property("airtightness", &Object::GetAirtightness, &Object::SetAirtightness)
		.def_property("invisibility", &Object::GetInvisibility, &Object::SetInvisibility)
		.def_property("tile", &Object::GetTile, &Object::SetTile)
		.def_property("position", &Object::GetPosition, &Object::SetPosition)
		.def_property("speed", &Object::GetSpeed, &Object::SetSpeed)
		.def_property("moveSpeed", &Object::GetMoveSpeed, &Object::SetMoveSpeed)
		.def_property("isFloor", &Object::IsFloor, &Object::SetIsFloor)
		.def_property("isWall", &Object::IsWall, &Object::SetIsWall)
		.def("AddVerb", &Object::AddVerb)
		.def("Update", &Object::Update)
		.def("InteractedBy", &Object::InteractedBy)
		.def("IsCloseTo", &Object::IsCloseTo)
		.def("Move", &Object::Move)
		.def("MoveZ", &Object::MoveZ)
		.def("AddComponent", (void (Object::*)(const std::string &)) &Object::AddComponent)
		.def("GetComponent", (Component *(Object::*)(const std::string &)) &Object::GetComponent)
		.def("AddObject", &Object::AddObject)
		.def("RemoveObject", &Object::RemoveObject, py::return_value_policy::reference)
		.def("SetSpriteState", &Object::SetSpriteState)
		.def("PlayAnimation", &Object::PlayAnimation, "Play animation",
			 py::arg("animation"), py::arg("callback") = nullptr)
		.def("Delete", &Object::Delete)
		.def("_updateIcons", &Object::updateIcons)
		.def("_pushToIcons", &Object::pushToIcons);

	py::class_<Map>(m, "Map")
		.def("GetTile", &Map::GetTile, py::return_value_policy::reference);

	m.def("CreateObject", &CreateObject);

	py::class_<Component, se::PyComponent>(m, "Component")
		.def(py::init<std::string &&>())
		.def("Update", &Component::Update)
		.def("GetOwner", &Component::GetOwner, "", py::return_value_policy::reference); // TODO: remove policy, when all objects will be implemented in scripts

	py::class_<ControlUIElement, std::shared_ptr<ControlUIElement>>(m, "ControlUIElement")
		.def(py::init<>())
		.def_property("position", &ControlUIElement::GetPosition, &ControlUIElement::SetPosition)
		.def("AddIcon", &ControlUIElement::AddIcon)
		.def("ClearIcons", &ControlUIElement::ClearIcons);

	py::class_<ControlUI>(m, "ControlUI")
		.def("UpdateElement", &ControlUI::UpdateElement)
		.def("RemoveElement", &ControlUI::RemoveElement);

	py::class_<Control, Component>(m, "Control")
		.def_property_readonly("ui", &Control::GetUI)
		.def_property("seeInvisibleAbility", &Control::GetSeeInvisibleAbility, &Control::SetSeeInvisibleAbility)
		.def("GetAndDropMoveOrder", &Control::GetAndDropMoveOrder)
		.def("GetAndDropMoveZOrder", &Control::GetAndDropMoveZOrder)
		.def("GetAndDropClickedObject", &Control::GetAndDropClickedObject, py::return_value_policy::reference);

	py::class_<Game>(m, "Game")
		.def("AddDelayedActivity", &Game::AddDelayedActivity);

	py::class_<IconInfo>(m, "Icon");

	py::class_<ResourceManager>(m, "ResourceManager")
		.def("GetIcon", &ResourceManager::GetIconInfo);

	py::class_<IServer>(m, "Server")
		.def_property_readonly_static("RM", [](py::object) { return IServer::RM(); }, py::return_value_policy::reference);

	m.attr("GGame") = GGame;
}

PYBIND11_EMBEDDED_MODULE(Shared, m) {
	py::enum_<Global::ItemSpriteState>(m, "ItemSpriteState")
		.value("DEFAULT",       Global::ItemSpriteState::DEFAULT)
		.value("ON_MOB",        Global::ItemSpriteState::ON_MOB)
		.value("IN_HAND_LEFT",  Global::ItemSpriteState::IN_HAND_LEFT)
		.value("IN_HAND_RIGHT", Global::ItemSpriteState::IN_HAND_RIGHT);
}

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
		py::module::import("EngineHook").attr("OnPlayerJoined")(player);
	} catch (const std::exception &e) {
		MANAGE_EXCEPTION(e);
	}
}
