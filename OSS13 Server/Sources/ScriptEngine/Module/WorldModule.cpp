#include <Python.h>

#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <plog/Log.h>

#include "../Trampoline/PyObject.h"
#include "../Trampoline/PyComponent.h"

#include <Player.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/CreateObject.h>
#include <World/Objects/Object.hpp>
#include <World/Objects/Control.hpp>
#include <World/Objects/ControlUI.h>

namespace py = pybind11;
namespace se = script_engine;

PYBIND11_EMBEDDED_MODULE(Engine_World, m) {
	py::class_<World>(m, "World")
		.def("GetObjectById", &World::GetObject)
		.def("GetMap", &World::GetMap);

	py::class_<Map>(m, "Map")
		.def_property_readonly("size", &Map::GetSize)
		.def("GetTile", &Map::GetTile, py::return_value_policy::reference);

	py::class_<Tile>(m, "Tile")
		.def_property_readonly("x", &Tile::X)
		.def_property_readonly("y", &Tile::Y)
		.def_property_readonly("z", &Tile::Z)
		.def_property_readonly("pos", &Tile::GetPos)
		.def_property_readonly("map", &Tile::GetMap)
		.def("IsDense", &Tile::IsDense)
		.def("IsSpace", &Tile::IsSpace)
		.def("GetDenseObject", &Tile::GetDenseObject, py::return_value_policy::reference);

	m.def("CreateObject", &CreateObject);

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

	py::class_<VerbsHolder>(m, "VerbHolder")
		.def("AddVerb", &VerbsHolder::AddVerb);

	py::class_<Component, se::PyComponent>(m, "Component")
		.def(py::init<std::string &&>())
		.def("Update", &Component::Update)
		.def("GetOwner", &Component::GetOwner, "", py::return_value_policy::reference); // TODO: remove policy, when all objects will be implemented in scripts

	py::class_<ControlUIElement, std::shared_ptr<ControlUIElement>>(m, "ControlUIElement")
		.def(py::init<>())
		.def_property("id", &ControlUIElement::GetId, &ControlUIElement::SetId)
		.def_property("position", &ControlUIElement::GetPosition, &ControlUIElement::SetPosition)
		.def("RegisterCallback", &ControlUI::RegistrateCallback)
		.def("AddIcon", &ControlUIElement::AddIcon)
		.def("PopIcon", &ControlUIElement::PopIcon)
		.def("ClearIcons", &ControlUIElement::ClearIcons);

	py::class_<ControlUI>(m, "ControlUI")
		.def_property_readonly("resolution", &ControlUI::GetResolution)
		.def_property_readonly("center", &ControlUI::GetCenter)
		.def_property_readonly("iconSize", &ControlUI::GetIconSize)
		.def("UpdateElement", &ControlUI::UpdateElement)
		.def("RemoveElement", &ControlUI::RemoveElement);

	py::class_<Control, Component>(m, "Control")
		.def_property_readonly("ui", &Control::GetUI)
		.def_property("seeInvisibleAbility", &Control::GetSeeInvisibleAbility, &Control::SetSeeInvisibleAbility)
		.def("GetAndDropMoveOrder", &Control::GetAndDropMoveOrder)
		.def("GetAndDropMoveZOrder", &Control::GetAndDropMoveZOrder)
		.def("GetAndDropClickedObject", &Control::GetAndDropClickedObject, py::return_value_policy::reference);
}
