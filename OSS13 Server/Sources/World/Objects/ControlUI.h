#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <unordered_map>

#include <Shared/Network/Protocol/ServerToClient/ControlUIData.h>
#include <Shared/Geometry/Vec2.hpp>

class Control;
class ControlUI;

class ControlUIElement : protected network::protocol::ControlUIData
{
public:
	void OnClick();
	void RegistrateCallback(std::function<void()> callback);

	uf::vec2i GetPosition() const;
	void SetPosition(uf::vec2i pos);

	void AddIcon(const std::string &icon);
	void PopIcon();
	void ClearIcons();

	void SetId(const std::string &id);
	const std::string &GetId() const;

	void SetControlUI(ControlUI *parent);
	ControlUI *GetControlUI() const;

	void NeedUpdate();
	bool GetAndDropUpdatedState();

protected:
	ControlUI *parent{nullptr};
	bool updated{false};
	std::function<void()> callback;
};

class ControlUI : public ControlUIElement
{
public:
	ControlUI(Control *control);

	void Update(std::chrono::microseconds timeElapsed);

	void OnClick(const std::string &key);

	void UpdateElement(std::shared_ptr<ControlUIElement> element);
	void RemoveElement(const std::string &key);

	void NeedUpdate();
	void NeedFullRefresh();

	uf::vec2i GetResolution() const;
	uf::vec2i GetCenter() const;
	uf::vec2i GetIconSize() const;

private:
	Control *control;
	bool synched{false};
	std::unordered_map<std::string, std::shared_ptr<ControlUIElement>> elements;
};
