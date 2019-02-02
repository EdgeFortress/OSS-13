#include "ResourceManager.hpp"

#include <fstream>

#include <Server.hpp>

#include <Shared/OS.hpp>
#include <Shared/JSON.hpp>

ResourceManager::ResourceManager() {
    using json = nlohmann::json;
    uint32_t lastSpriteNum = 0;

    // Load icons list
    auto config_files = FindFilesRecursive(L"Resources/Icons", L"*.json");
    for (const auto &config_file_path: config_files) {
        std::ifstream config_istr(config_file_path);
        json config;
        config_istr >> config;
        for (auto sprite_config : config["sprites"]) {
            lastSpriteNum++;
            auto title = sprite_config.find("sprite");
            if (title != sprite_config.end()) {
                sprites[title->get<std::string>()] = lastSpriteNum;
            }
        }
    }

	Server::log << "ResourceManager created." << lastSpriteNum << "sprites loaded!" << std::endl;
}

uint32_t ResourceManager::GetSpriteNum(const std::string &sprite, Global::ItemSpriteState state) {
    const auto iter = sprites.find(sprite);
    if (iter != sprites.end()) {
        return iter->second + uint32_t(state);
    }
    Server::log << "ResourceManager::GetSpriteNum miss. Sprite" << sprite << std::endl;
    return 0;
}
