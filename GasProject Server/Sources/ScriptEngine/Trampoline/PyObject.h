#pragma once

#include <pybind11/pytypes.h>
#include <pybind11/chrono.h>

#include <IGame.h>
#include <World/World.hpp>
#include <World/Objects/Object.hpp>

namespace py = pybind11;

namespace script_engine {

class PyObject : public Object, public std::enable_shared_from_this<PyObject> {
public:
	void Update(std::chrono::microseconds timeElapsed) override {
		Object::Update(timeElapsed);
		PYBIND11_OVERLOAD_PURE_NAME(void, Object, "Update", Update, timeElapsed);
	}

	bool InteractedBy(Object *obj) override {
		PYBIND11_OVERLOAD_PURE_NAME(bool, Object, "InteractedBy", InteractedBy, obj);
	}

	bool RemoveObject(Object *obj) override {
		PYBIND11_OVERLOAD_NAME(bool, Object, "RemoveObject", RemoveObject, obj);
	}

	void Delete() final {
		Object::Delete();
		LOGI << "Released";
	}

	void updateIcons() override {
		PYBIND11_OVERLOAD_NAME(void, Object, "_updateIcons", updateIcons);
	}

	void SetImpl() {
		this->pyImpl = py::cast(this); // Get Python object. Here cycling link is created. We should broke it in Delete method.
		GGame->GetWorld()->AddObject(shared_from_this()); // Add object to ObjectHolder
	}

private:
	py::object pyImpl;
};

} // namespace script_engine

// Special pointer with hook for object intercepting and binding it to ObjectHolder
// We need link C++ object with Python object by SetImpl call on pointer creation
template <typename T>
class PyObjectPtr {
private:
	std::shared_ptr<T> _impl;

public:
	using element_type = T;

	PyObjectPtr(T *obj) { // obj - C++ object corresponding to Python object
		if (auto *trampoline = dynamic_cast<script_engine::PyObject *>(obj)) {
			_impl.reset(trampoline);
			trampoline->SetImpl();
		} else {
			_impl.reset(obj); // TODO: remove this, when all objects are implemented in scripts
		}
	}

	PyObjectPtr(PyObjectPtr &&) = default;

	PyObjectPtr(std::shared_ptr<T> r) : _impl(r) {}
	operator std::shared_ptr<T>() { return _impl; }
	T* get() const { return _impl.get(); }
};

PYBIND11_DECLARE_HOLDER_TYPE(T, PyObjectPtr<T>);
