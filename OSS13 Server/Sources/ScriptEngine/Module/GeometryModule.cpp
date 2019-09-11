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
		.def_readwrite("y", &uf::vec2<T>::y)
		.def("Length", &uf::vec2<T>::length)
		.def("Normalize", &uf::vec2<T>::normalize)
		.def("GetAngle", &uf::vec2<T>::GetAngle)
		.def("GetDirection", [](const uf::vec2<T> &v) { return VectToDirection(v); })
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
void RegistrateSpaceVector(py::module m, const char *name) {
	py::class_<uf::vec3<T>>(m, name)
		.def(py::init<>())
		.def(py::init<T, T, T>())
		.def(py::init<uf::vec2<T>, T>())
		.def(py::init<T>())
		.def_readwrite("x", &uf::vec3<T>::x)
		.def_readwrite("y", &uf::vec3<T>::y)
		.def_readwrite("z", &uf::vec3<T>::z)
		.def_property_readonly("xy", &uf::vec3<T>::xy)
		.def("Length", &uf::vec3<T>::length)
		.def("Normalize", &uf::vec3<T>::normalize)
		.def("GetDirection", [](const uf::vec3<T> &v) { return VectToDirection(v); })
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self * int())
		.def(py::self * float())
		.def(py::self / int())
		.def(py::self / float())
		.def("__repr__", &uf::vec3<T>::toString)
		.def("__bool__", &uf::vec3<T>::operator bool);
}

PYBIND11_EMBEDDED_MODULE(Engine_Geometry, m) {
	RegistratePlaneVector<int32_t>(m, "Vec2i");
	RegistratePlaneVector<double>(m, "Vec2f");
	RegistrateSpaceVector<int32_t>(m, "Vec3i");
	RegistrateSpaceVector<double>(m, "Vec3f");

	// compatibility, don't use it
	RegistratePlaneVector<uint32_t>(m, "Vec2i_unsigned");
	RegistratePlaneVector<float>(m, "Vec2f_32");

	py::enum_<uf::Direction>(m, "Direction")
		.value("NONE", uf::Direction::NONE)
		.value("SOUTH", uf::Direction::SOUTH)
		.value("WEST", uf::Direction::WEST)
		.value("NORTH", uf::Direction::NORTH)
		.value("EAST", uf::Direction::EAST)
		.value("SOUTH_WEST", uf::Direction::SOUTH_WEST)
		.value("NORTH_WEST", uf::Direction::NORTH_WEST)
		.value("NORTH_EAST", uf::Direction::NORTH_EAST)
		.value("SOUTH_EAST", uf::Direction::SOUTH_EAST)
		.value("CENTER", uf::Direction::CENTER);

	m.def("DirectionToVect", &uf::DirectionToVect);
	m.def("InvertDirection", &uf::InvertDirection);
	m.def("SplitDirection", &uf::SplitDirection);

	py::class_<uf::DirectionSet>(m, "DirectionSet")
		.def(py::init<>())
		.def("Add", (void (uf::DirectionSet::*)(const std::list<uf::Direction> &)) &uf::DirectionSet::Add)
		.def("Remove", (void (uf::DirectionSet::*)(const std::list<uf::Direction> &)) &uf::DirectionSet::Remove)
		.def("IsExistsOne", (bool (uf::DirectionSet::*)(const std::list<uf::Direction> &) const) &uf::DirectionSet::IsExistsOne)
		.def("AreExistAll", (bool (uf::DirectionSet::*)(const std::list<uf::Direction> &) const) &uf::DirectionSet::AreExistAll)
		.def("Reset", &uf::DirectionSet::Reset);

	py::class_<uf::DirectionSetFractional>(m, "DirectionSetFractional")
		.def("Add", &uf::DirectionSetFractional::Add);
}
