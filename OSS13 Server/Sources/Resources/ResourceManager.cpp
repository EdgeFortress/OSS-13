#include "ResourceManager.hpp"

#include <fstream>
#include <filesystem>

#include <plog/Log.h>

#include <IServer.h>

#include <Shared/OS.hpp>
#include <Shared/ErrorHandling.h>

using json = nlohmann::json;
using namespace std::chrono_literals;

bool ResourceManager::Initialize() {
	try {
		loadIcons();
	} catch (std::filesystem::filesystem_error e) {
		LOGE << e.what();
		return false;
	}
	return true;
}

void ResourceManager::loadIcons() {
	icons.clear();

	uint32_t lastIconNum = 0;

	// Load icons list
	auto config_files = FindFilesRecursive("Resources/Icons", "*.json");
	for (auto &config_file_path : config_files) {
		std::string configFilePathStr = config_file_path.string();
		std::ifstream config_istr(configFilePathStr);
		json config;
		config_istr >> config;
		for (auto icon_config : config["sprites"]) {
			lastIconNum++;
			auto title = icon_config.find("sprite");
			if (title != icon_config.end()) {
				IconInfo iconInfo = parseIconInfo(icon_config);

				iconInfo.id = lastIconNum;
				iconInfo.title = title->get<std::string>();

				icons[iconInfo.title] = std::move(iconInfo);
			}
		}
	}

	LOGI << "ResourceManager created. " << lastIconNum << " sprites loaded!";
}

IconInfo ResourceManager::parseIconInfo(const json &icon_config) {
	IconInfo iconInfo;

	auto frames = icon_config.find("frames");
	if (frames != icon_config.end()) {
		iconInfo.isAnimation = true;
		auto loop_time = icon_config.find("loop_time");
		if (loop_time != icon_config.end())
			iconInfo.animation_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<float>(loop_time->get<float>()));
		else
			iconInfo.animation_time = frames->get<int>() * 100us;
	}

	return iconInfo;
}

IconInfo ResourceManager::GetIconInfo(const std::string &title, Global::ItemSpriteState state) {
	auto iter = icons.find(title);
	if (iter != icons.end()) {
		auto iconInfo = iter->second;
		iconInfo.state = state;
		return iconInfo;
	}
	EXPECT_WITH_MSG(false, "Unknown icon: \"" + title + "\"");
}
