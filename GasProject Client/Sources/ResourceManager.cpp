#include "ResourceManager.hpp"

#include <fstream>

#include <Client.hpp>
#include <Graphics/Sprite.hpp>

#include <Shared/JSON.hpp>
#include <Shared/OS.hpp>

using json = nlohmann::json;

ResourceManager::ResourceManager() { }

void ResourceManager::Initialize() {
	// Load icons list
	auto config_files = FindFilesRecursive(IMAGE_CONFIGS_PATH, IMAGE_CONFIG_MASK);
	for (const auto &config_file_path : config_files) {
		generateTexturesAndSprites(config_file_path);
	}
}

void ResourceManager::generateTexturesAndSprites(const std::wstring &configpath) {
	FileInfo configPathParseResult = ParseFilePath(configpath);

	json config;
	std::ifstream(configpath) >> config;

	uint32_t tileSize = config["tileSize"];

	Texture *texture = nullptr;
	Texture *mobStateTexture = nullptr;
	Texture *lhandStateTexture = nullptr;
	Texture *rhandStateTexture = nullptr;

	texture = LoadTexture(configPathParseResult.path + L"/" + configPathParseResult.name + L".png", tileSize);

	if (config.find("mobState") != config.end())
		mobStateTexture = LoadTexture(configPathParseResult.path + L"/" + configPathParseResult.name + L".mob.png", tileSize);
	if (config.find("lhandState") != config.end())
		lhandStateTexture = LoadTexture(configPathParseResult.path + L"/" + configPathParseResult.name + L".lhand.png", tileSize);
	//if (config.find("rhandState") != config.end())
	//	rhandStateTexture = LoadTexture(configPathParseResult.path + L"/" + configPathParseResult.name + L".rhand.png", tileSize);

	generateSprites(texture, mobStateTexture, lhandStateTexture, rhandStateTexture, config);
}

void ResourceManager::generateSprites(
	const Texture *texture, 
	const Texture *mobState_texture, 
	const Texture *lhandState_texture,
	const Texture *rhandState_texture, 
	json &config) 
{
	std::string title; // Can be same for few sequential sprites
	uint firstFrame = 0;
	uint mobState_firstFrame = 0;
	uint lhandState_firstFrame = 0;
	uint rhandState_firstFrame = 0;
	for (auto sprite_config : config["sprites"]) {
		SpriteInfo spriteInfo;

		spriteInfo.texture = texture;
		spriteInfo.mobState_texture = mobState_texture;
		spriteInfo.lhandState_texture = lhandState_texture;
		spriteInfo.rhandState_texture = rhandState_texture;

		// Sprite title
		auto iter = sprite_config.find("sprite");
		if (iter != sprite_config.end()) {
			title = iter->get<std::string>();
		}
		spriteInfo.title = title;

		// Frames
		iter = sprite_config.find("frames");
		if (iter != sprite_config.end()) spriteInfo.frames = iter->get<int>();
		else spriteInfo.frames = 1;

		// Frame Time
		iter = sprite_config.find("frame_time");
		if (iter != sprite_config.end()) spriteInfo.frameTime = sf::seconds(iter->get<float>());
		else {
			iter = sprite_config.find("loop_time");
			if (iter != sprite_config.end())
				spriteInfo.frameTime = sf::seconds(iter->get<float>() / spriteInfo.frames);
			else
				spriteInfo.frameTime = sf::seconds(0.1f);
		}

		// Directed
		iter = sprite_config.find("directed");
		if (iter != sprite_config.end()) spriteInfo.directed = iter->get<bool>();
		else spriteInfo.directed = false;

		spriteInfo.firstFrame = firstFrame;
		if (mobState_texture)
			spriteInfo.mobState_firstFrame = mobState_firstFrame;
		if (lhandState_texture)
			spriteInfo.lhandState_firstFrame = lhandState_firstFrame;
		if (rhandState_texture)
			spriteInfo.rhandState_firstFrame = rhandState_firstFrame;

		firstFrame += spriteInfo.frames * (int(spriteInfo.directed) * 3 + 1);
		if (mobState_texture)
			mobState_firstFrame += 4;
		if (lhandState_texture)
			lhandState_firstFrame += 4;
		if (rhandState_texture)
			rhandState_firstFrame += 4;

		recipes.push_back(std::move(spriteInfo));
	}
}
