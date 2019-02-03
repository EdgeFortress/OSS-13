#pragma once

#include <vector>

#include <Shared/JSON.hpp>

#include <Graphics/TextureHolder.h>
#include <Graphics/SpriteFactory.h>

class ResourceManager : 
	public TextureHolder,
	public SpriteFactory
{
public:
    ResourceManager();

	void Initialize();

private:
	void generateTexturesAndSprites(const std::wstring &configPath);
	void generateSprites(const Texture *texture, const Texture *mobStateTexture, nlohmann::json &config);

private:
	const wchar_t *IMAGE_CONFIGS_PATH = L"Resources/Icons";
	const wchar_t *IMAGE_CONFIG_MASK = L"*.json";

    uint32_t lastSpriteNum;
};
