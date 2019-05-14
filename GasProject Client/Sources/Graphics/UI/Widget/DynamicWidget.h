#pragma once

#include <unordered_map>
#include <queue>
#include <any>

#include <Graphics/UI/Widget/ImGuiWidget.h>

#include <Shared/JSON.hpp>
#include <Shared/Network/Protocol/InputData.h>

using json = nlohmann::json;

class ImGuiInput {
public:
	ImGuiInput() :
		isChanged(false)
	{ }

	template<class T>
	T GetData() {
		if (!uiData.has_value())
			uiData = T();
		return std::any_cast<T>(uiData);
	}

	template<class T>
	void SetData(const T &data) {
		if (std::any_cast<T>(uiData) != data) {
			uiData = data;
			isChanged = true;
		}
	}

	bool IsChanged() const {
		return isChanged;
	}

	uptr<UIData> GetInputData() {
		auto res = std::make_unique<RadioButtonUIData>();
		res->data = std::any_cast<int>(uiData);
		isChanged = false;
		return res;
	}

private:
	std::any uiData;
	std::any serverData;
	mutable bool isChanged;
};

struct ImGuiTrigger {
	std::string trigger;
};

class DynamicWidget : public ImGuiWidget {
public:
	DynamicWidget(const char *layout);

	void Update(sf::Time timeElapsed);

private:
	void render();
	void sendUpdates();

	void renderTest(json &layout);
	void renderText(json &layout);
	void renderButton(json &layout);
	void renderRadio(json &layout);

private:
	json layout;

	std::string id;

	std::unordered_map<std::string, ImGuiInput> handles;
	std::queue<ImGuiTrigger> triggers;
};
