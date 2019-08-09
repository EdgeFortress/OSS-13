#pragma once

#include <World/Objects/Component.hpp>

namespace script_engine {

class PyComponent : public Component {
	using Component::Component;

	void Update(std::chrono::microseconds timeElapsed) override {
		PYBIND11_OVERLOAD_PURE_NAME(void, Component, "Update", Update, timeElapsed);
	}
};

} // namespace script_engine
