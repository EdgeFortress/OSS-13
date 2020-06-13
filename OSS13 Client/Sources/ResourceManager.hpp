#pragma once

#include <vector>

#include <Shared/JSON.hpp>
#include <Shared/ConfigController.h>

#include <Graphics/TextureHolder.h>
#include <Graphics/SpriteFactory.h>

class ResourceManager : 
	public TextureHolder,
	public SpriteFactory
{
public:
    ResourceManager();

	void Initialize();

	const IConfig *Config() const;

private:
	void generateTexturesAndSprites(const std::string &configPath);
	void generateSprites(
		const Texture *texture,
		const Texture *mobState_texture,
		const Texture *lhandState_texture,
		const Texture *rhandState_texture,
		nlohmann::json &config);

private:
	const char *IMAGE_CONFIGS_PATH = "Resources/Icons";
	const char *IMAGE_CONFIG_MASK = "*.json";
	const char *CONFIG_FILE = "Resources/ClientConfig.json";

	ConfigController configController;

    uint32_t lastSpriteNum;
};
