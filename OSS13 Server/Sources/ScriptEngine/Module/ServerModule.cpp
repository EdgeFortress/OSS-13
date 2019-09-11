#include <Python.h>

#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
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
	py::class_<IServer>(m, "Server")
		.def_property_readonly_static("RM", [](py::object) { return IServer::RM(); }, py::return_value_policy::reference);
	m.attr("GServer") = GServer;

	py::class_<Game>(m, "Game")
		.def_property_readonly("world", &Game::GetWorld)
		.def("AddDelayedActivity", &Game::AddDelayedActivity);
	m.attr("GGame") = GGame;
		
	py::class_<Player, VerbsHolder>(m, "Player")
		.def_property_readonly("ckey", &Player::GetCKey)
		.def_property("control", &Player::GetControl, &Player::SetControl, "", py::return_value_policy::reference)
		.def("IsConnected", &Player::IsConnected);

	py::class_<ResourceManager>(m, "ResourceManager")
		.def("GetIcon", &ResourceManager::GetIconInfo);

	py::class_<IconInfo>(m, "Icon");

	py::enum_<Global::ItemSpriteState>(m, "ItemSpriteState")
		.value("DEFAULT", Global::ItemSpriteState::DEFAULT)
		.value("ON_MOB", Global::ItemSpriteState::ON_MOB)
		.value("IN_HAND_LEFT", Global::ItemSpriteState::IN_HAND_LEFT)
		.value("IN_HAND_RIGHT", Global::ItemSpriteState::IN_HAND_RIGHT);
}
