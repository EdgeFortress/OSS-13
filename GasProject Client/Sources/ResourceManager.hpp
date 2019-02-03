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
	void generateSprites(
		const Texture *texture,
		const Texture *mobState_texture,
		const Texture *lhandState_texture,
		const Texture *rhandState_texture,
		nlohmann::json &config);

private:
	const wchar_t *IMAGE_CONFIGS_PATH = L"Resources/Icons";
	const wchar_t *IMAGE_CONFIG_MASK = L"*.json";

    uint32_t lastSpriteNum;
};
