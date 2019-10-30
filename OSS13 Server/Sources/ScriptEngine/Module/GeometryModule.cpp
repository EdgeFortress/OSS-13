#include <Python.h>

#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <Shared/Geometry/Vec2.hpp>
#include <Shared/Geometry/Vec3.hpp>
#include <Shared/Geometry/Direction.hpp>
#include <Shared/Geometry/DirectionSet.h>

namespace py = pybind11;

template<typename T>
void RegistratePlaneVector(py::module m, const char *name) {
	py::class_<uf::vec2<T>>(m, name)
		.def(py::init<>())
		.def(py::init<T, T>())
		.def(py::init<T>())
		.def_readwrite("x", &uf::vec2<T>::x)
		.def_readwrite("y", &uf::vec2<T>::y);
}

template<typename T>
void RegistrateSpaceVector(py::module m, const char *name) {
	py::class_<uf::vec3<T>>(m, name)
		.def(py::init<>())
		.def(py::init<T, T, T>())
		.def(py::init<uf::vec2<T>, T>())
		.def(py::init<T>())
		.def_readwrite("x", &uf::vec3<T>::x)
		.def_readwrite("y", &uf::vec3<T>::y)
		.def_readwrite("z", &uf::vec3<T>::z);
}

PYBIND11_EMBEDDED_MODULE(Engine_Geometry, m) {
	RegistratePlaneVector<int32_t>(m, "eVec2i");
	RegistratePlaneVector<double>(m, "eVec2f");
	RegistrateSpaceVector<int32_t>(m, "eVec3i");
	RegistrateSpaceVector<double>(m, "eVec3f");

	// compatibility, don't use it
	RegistratePlaneVector<uint32_t>(m, "eVec2i_unsigned");
	RegistratePlaneVector<float>(m, "eVec2f_32");

	py::enum_<uf::Direction>(m, "eDirection")
		.value("NONE", uf::Direction::NONE)
		.value("SOUTH", uf::Direction::SOUTH)
		.value("WEST", uf::Direction::WEST)
		.value("NORTH", uf::Direction::NORTH)
		.value("EAST", uf::Direction::EAST)
		.value("SOUTH_WEST", uf::Direction::SOUTH_WEST)
		.value("NORTH_WEST", uf::Direction::NORTH_WEST)
		.value("NORTH_EAST", uf::Direction::NORTH_EAST)
		.value("SOUTH_EAST", uf::Direction::SOUTH_EAST)
		.value("TOP", uf::Direction::TOP)
		.value("BOTTOM", uf::Direction::BOTTOM)
		.value("CENTER", uf::Direction::CENTER);

	m.def("eDirectionToVect", &uf::DirectionToVect);
	m.def("eInvertDirection", &uf::InvertDirection);

	py::class_<uf::DirectionSet>(m, "eDirectionSet")
		.def(py::init<>())
		.def("Add", (void (uf::DirectionSet::*)(const std::list<uf::Direction> &)) &uf::DirectionSet::Add)
		.def("Remove", (void (uf::DirectionSet::*)(const std::list<uf::Direction> &)) &uf::DirectionSet::Remove)
		.def("DoesExistOne", (bool (uf::DirectionSet::*)(const std::list<uf::Direction> &) const) &uf::DirectionSet::DoesExistOne)
		.def("DoExistAll", (bool (uf::DirectionSet::*)(const std::list<uf::Direction> &) const) &uf::DirectionSet::DoExistAll)
		.def("Reset", &uf::DirectionSet::Reset);

	py::class_<uf::DirectionSetFractional>(m, "eDirectionSetFractional")
		.def(py::init<>())
		.def("Set", (void (uf::DirectionSetFractional::*)(const std::list<uf::DirectionFractional> &fractDirections)) &uf::DirectionSetFractional::Set)
		.def("Remove", (void (uf::DirectionSetFractional::*)(const std::list<uf::Direction> &fractDirections)) &uf::DirectionSetFractional::Remove)
		.def("GetFraction", &uf::DirectionSetFractional::GetFraction)
		.def("GetCumulativeFraction", &uf::DirectionSetFractional::GetCumulativeFraction)
		.def("Reset", &uf::DirectionSetFractional::Reset);
}
