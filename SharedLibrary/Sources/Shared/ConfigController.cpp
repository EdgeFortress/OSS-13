#include "ConfigController.h"

#include <fstream>
#include <plog/Log.h>

void ConfigController::Load(const std::wstring &configPath) {
	std::ifstream(std::string(configPath.begin(), configPath.end())) >> config;

	LOGD << "Config loaded:";

	for (auto section = config.begin(); section != config.end(); ++section) {
		LOGD << "\tSection: " << section.key();
		for (auto option = section.value().begin(); option != section.value().end(); ++option) {
			LOGD << "\t\tOption: " << option.key() << " = " << option.value();
		}
	}
}

const json &GetJsonNodeByPath(const json &j, const char *path) {
	std::string key(path);

	auto node = &j;
	size_t pos = 0;
	std::string token;
	while ((pos = key.find('.')) != std::string::npos) {
		token = key.substr(0, pos);
		node = &(*node)[token];
		key.erase(0, pos + 1);
	}

	return (*node)[key];
}

template<class T>
T GetValueFromJson(const json &j, const char *path) {
	auto &node = GetJsonNodeByPath(j, path);
	return node.get<T>();
}

template<class T>
uf::vec2<T> GetVectorFromJson(const json &j, const char *path) {
	auto &node = GetJsonNodeByPath(j, path);
	return {node[0].get<T>(), node[1].get<T>()};
}

std::string ConfigController::GetString(const char *option) const { return std::move(GetValueFromJson<std::string>(config, option)); }
int ConfigController::GetInt(const char *option) const { return GetValueFromJson<int>(config, option); }
bool ConfigController::GetBool(const char *option) const { return GetValueFromJson<bool>(config, option); }
uf::vec2i ConfigController::GetVec2i(const char *option) const { return GetVectorFromJson<int>(config, option); }
uf::vec2f ConfigController::GetVec2f(const char *option) const { return GetVectorFromJson<float>(config, option); }
