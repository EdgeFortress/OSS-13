#pragma once

#include <vector>
#include <unordered_map>

#include <Shared/Geometry/Vec2.hpp>

class ControlUIElement
{
public:
	uf::vec2i GetPosition() const;
	void SetPosition(uf::vec2i pos);

	void AddIcon(const std::string &icon);
	void ClearIcons();

private:
	bool updated{false};
	uf::vec2i position;
	std::vector<std::string> icons;
};

class ControlUI
{
public:
	void UpdateElement(const std::string &key, std::shared_ptr<ControlUIElement> element) {
		elements[key] = std::move(element);
	}

	void RemoveElement(const std::string &key) {
		elements.erase(key);
	}

private:
	std::unordered_map<std::string, std::shared_ptr<ControlUIElement>> elements;
};
