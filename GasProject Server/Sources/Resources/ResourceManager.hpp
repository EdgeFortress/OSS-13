#pragma once

#include <unordered_map>

#include <Shared/Global.hpp>
#include <Shared/JSON.hpp>

#include "IconInfo.h"

class ResourceManager {
public:
    ResourceManager() = default;
	void AfterCreation();

	IconInfo GetIconInfo(const std::string &spriteName, Global::ItemSpriteState spriteState = Global::ItemSpriteState::DEFAULT);

private:
	void loadIcons();
	IconInfo parseIconInfo(const nlohmann::json &icon_config);

private:
    std::unordered_map<std::string, IconInfo> icons;
    std::unordered_map<std::string, IconInfo> sounds;
};
