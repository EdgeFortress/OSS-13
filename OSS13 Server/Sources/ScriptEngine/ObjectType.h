#pragma once

#include <Python.h>
#include <pybind11/embed.h>

#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>

namespace py = pybind11;

class ObjectType : public network::protocol::ObjectType {
public:
	explicit ObjectType(py::handle cls);

	bool CanBeCreatedByEngine() const;
	bool CanBeSpawned() const;
	const std::string &GetName() const;
	const std::string &GetTypeKey() const;
	py::handle GetHandle();
	py::handle GetModule();

private:
	bool canBeCreatedByEngine{true};
	bool canBeSpawned{false};
	py::handle cls;
	py::handle module;
};
