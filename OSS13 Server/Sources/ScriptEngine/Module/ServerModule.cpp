#include <Python.h>

#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include <plog/Log.h>

#include <IServer.h>
#include <Game.h>
#include <Player.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/Control.hpp>
#include <Resources/ResourceManager.hpp>

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(Engine_Server, m) {
	py::class_<IServer>(m, "eServer")
		.def_property_readonly_static("RM", [](py::object) { return IServer::RM(); }, py::return_value_policy::reference);
	m.attr("eGServer") = GServer;

	py::class_<Game>(m, "eGame")
		.def_property_readonly("world", &Game::GetWorld)
		.def("AddDelayedActivity", &Game::AddDelayedActivity);
	m.attr("eGGame") = GGame;

	py::class_<VerbsHolder>(m, "eVerbsHolder")
		.def("AddVerb", &VerbsHolder::AddVerb);
		
	py::class_<Player, VerbsHolder>(m, "ePlayer")
		.def_property_readonly("ckey", &Player::GetCKey)
		.def_property("control", &Player::GetControl, &Player::SetControl, "", py::return_value_policy::reference)
		.def("IsConnected", &Player::IsConnected);

	py::class_<ResourceManager>(m, "eResourceManager")
		.def("GetIcon", &ResourceManager::GetIconInfo);

	py::class_<IconInfo>(m, "eIcon");

	py::enum_<Global::ItemSpriteState>(m, "eItemSpriteState")
		.value("DEFAULT", Global::ItemSpriteState::DEFAULT)
		.value("ON_MOB", Global::ItemSpriteState::ON_MOB)
		.value("IN_HAND_LEFT", Global::ItemSpriteState::IN_HAND_LEFT)
		.value("IN_HAND_RIGHT", Global::ItemSpriteState::IN_HAND_RIGHT);
}
