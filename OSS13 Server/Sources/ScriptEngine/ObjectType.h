#pragma once

#include <Python.h>
#include <pybind11/embed.h>

namespace py = pybind11;

class ObjectType {
public:
	explicit ObjectType(py::handle cls);

	const std::string &GetName();
	const std::string &GetTypeKey();
	py::handle GetHandle();
	py::handle GetModule();

private:
	std::string name;
	std::string typeKey;
	py::handle cls;
	py::handle module;
};
