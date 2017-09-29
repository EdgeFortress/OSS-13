#include "ResourceManager.hpp"

#include <fstream>

#include "Shared/OS.hpp"
#include "Shared/JSON.hpp"

#include "Server.hpp"

ResourceManager::ResourceManager() {
    using json = nlohmann::json;
    uint lastIconNum = 0;

    // Load icons list
    auto config_files = FindFilesRecursive(L"Resources/Icons", L"*.json");
    for (const auto &config_file_path: config_files) {
        std::ifstream config_istr(config_file_path);
        json config;
        config_istr >> config;
        for (auto sprite_config : config["sprites"]) {
            lastIconNum++;
            const auto title = sprite_config.find("sprite");
            if (title != sprite_config.end()) {
                icons[title->get<std::string>()] = lastIconNum;
            }
        }
    }
}

uint ResourceManager::GetIconNum(const std::string &icon) {
    const auto iter = icons.find(icon);
    if (iter != icons.end()) {
        return iter->second;
    }
    // Icon not founded
    return 0;
}
