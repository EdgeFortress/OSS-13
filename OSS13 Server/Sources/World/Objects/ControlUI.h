#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>

#include <Shared/Network/Protocol/ServerToClient/ControlUIData.h>
#include <Shared/Geometry/Vec2.hpp>

class Control;

class ControlUIElement : protected network::protocol::ControlUIData
{
public:
	uf::vec2i GetPosition() const;
	void SetPosition(uf::vec2i pos);

	void AddIcon(const std::string &icon);
	void ClearIcons();

	bool GetAndDropUpdatedState();

protected:
	bool updated{false};
};

class ControlUI : public ControlUIElement
{
public:
	ControlUI(Control *control);

	void Update(std::chrono::microseconds timeElapsed);

	void UpdateElement(const std::string &key, std::shared_ptr<ControlUIElement> element);
	void RemoveElement(const std::string &key);

private:
	Control *control;
	std::unordered_map<std::string, std::shared_ptr<ControlUIElement>> elements;
};
