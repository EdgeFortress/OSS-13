#pragma once

#include <Python.h>
#include <pybind11/embed.h>

#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

namespace py = pybind11;

class ObjectType : public network::protocol::ObjectType {
public:
	explicit ObjectType(py::handle cls);

	bool CanBeSpawned() const;
	const std::string &GetName() const;
	const std::string &GetTypeKey() const;
	py::handle GetHandle();
	py::handle GetModule();

private:
	bool canBeSpawned;
	py::handle cls;
	py::handle module;
};
