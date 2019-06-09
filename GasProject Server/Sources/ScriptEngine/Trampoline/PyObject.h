#pragma once

#include <pybind11/pytypes.h>

#include <World/Objects/Object.hpp>

namespace py = pybind11;

namespace script_engine {

class PyObject : public Object, public std::enable_shared_from_this<PyObject> {
public:
	void Update(sf::Time timeElapsed) override
	{
		Object::Update(timeElapsed);
		UpdateWithoutTime();
	}

	void UpdateWithoutTime() override {
		PYBIND11_OVERLOAD_PURE_NAME(void, Object, "Update", UpdateWithoutTime, );
	}

	void Delete() final {
		Object::Delete();
		if (pyImpl) {
			pyImpl.dec_ref();
			pyImpl.release();
		}
	}

	bool InteractedBy(Object *) override { return false; }

	void SetImpl(PyObject *pyImpl) {
		this->pyImpl = py::cast(pyImpl); // Get Python object. Here cycling link is created. We should broke it in Delete method.
		GGame->GetWorld()->AddObject(pyImpl->shared_from_this()); // Add object to ObjectHolder
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
		auto *trampoline = dynamic_cast<script_engine::PyObject *>(obj);
		_impl.reset(trampoline);
		trampoline->SetImpl(trampoline);
	}

	PyObjectPtr(PyObjectPtr &&) = default;

	PyObjectPtr(std::shared_ptr<T> r) : _impl(r) {}
	operator std::shared_ptr<T>() { return _impl; }
	T* get() const { return _impl.get(); }
};

PYBIND11_DECLARE_HOLDER_TYPE(T, PyObjectPtr<T>);
