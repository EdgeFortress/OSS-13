#include "ResourceManager.hpp"

#include <fstream>

#include "Graphics/Sprite.hpp"

#include "Shared/JSON.hpp"
#include "Shared/OS.hpp"

ResourceManager::ResourceManager() {
    using json = nlohmann::json;

    uint lastIconNum = 0;

    // Load icons list
    auto config_files = FindFilesRecursive(L"Resources/Icons", L"*.json");
    for (const auto &config_file_path : config_files) {
        std::ifstream config_istr(config_file_path);
        json config;
        config_istr >> config;

        const FileInfo configPathParseResult = ParseFilePath(config_file_path);
        const std::string texturePath = sf::String(configPathParseResult.path + L"/" + configPathParseResult.name + L".png");
        Texture *texture = new Texture(texturePath, config);
        textures.push_back(uptr<Texture>(texture));

        auto &texture_sprites = texture->GetSprites();
        for (auto &sprite: texture_sprites) {
            sprites.push_back(uptr<Sprite>(sprite));
        }
    }
}

void ResourceManager::SpritesNextFrame() {
    for (auto &sprite : sprites)
        sprite->UpdateFrame();
}

void ResourceManager::SpritesResize(uint size) {
    for (auto &sprite : sprites)
        sprite->Resize(size);
}

Sprite *ResourceManager::GetSprite(uint id) {
    if (id == 0) return nullptr;
    return sprites[id - 1].get();
}