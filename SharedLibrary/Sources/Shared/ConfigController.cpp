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

template<class T>
T GetValueFromJson(json j, const char *k) {
	std::string key(k);

	json iter = j;
	size_t pos = 0;
	std::string token;
	while ((pos = key.find('.')) != std::string::npos) {
		token = key.substr(0, pos);
		iter = iter[token];
		key.erase(0, pos + 1);
	}

	return iter[key].get<T>();
}

std::string ConfigController::GetString(const char *option) const { return std::move(GetValueFromJson<std::string>(config, option)); }
int ConfigController::GetInt(const char *option) const { return GetValueFromJson<int>(config, option); }
bool ConfigController::GetBool(const char *option) const { return GetValueFromJson<bool>(config, option); }
