#include "ResourceManager.hpp"

#include <fstream>

#include <Client.hpp>
#include <Graphics/Sprite.hpp>

#include <Shared/JSON.hpp>
#include <Shared/OS.hpp>

ResourceManager::ResourceManager() {
    using json = nlohmann::json;

    nextSpriteNum = 1;

    // Load icons list
    auto config_files = FindFilesRecursive(L"Resources/Icons", L"*.json");
    for (const auto &config_file_path : config_files) {
        std::ifstream config_istr(config_file_path);
        json config;
        config_istr >> config;

        const FileInfo configPathParseResult = ParseFilePath(config_file_path);
        const std::string texturePath = sf::String(configPathParseResult.path + L"/" + configPathParseResult.name + L".png");
        Texture *texture = new Texture(texturePath, config, nextSpriteNum);
        textures.push_back(uptr<Texture>(texture));

        uint numOfNewSprites = texture->GetNumOfSprites();
        for (uint i = 0; i < numOfNewSprites; i++) {
            spritesMap.push_back(texture);
        }

        nextSpriteNum += numOfNewSprites;
    }
}

Sprite ResourceManager::GetSprite(uint id) {
    if (!id || id >= nextSpriteNum) {
		CC::log << "ResourceManager::GetSprite miss. ID" << id << std::endl;
		return std::move(Sprite());
	}
    return std::move(spritesMap[id - 1]->GetSprite(id));
}
