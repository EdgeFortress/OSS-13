#pragma once

#include <Shared/IFaces/IConfig.h>
#include <Shared/JSON.hpp>

using json = nlohmann::json;

class ConfigController : public IConfig {
public:
	void Load(const std::wstring &configPath);

	std::string GetString(const char *option) const final;
	int GetInt(const char *option) const final;
	bool GetBool(const char *option) const final;
	uf::vec2i GetVec2i(const char *option) const final;
	uf::vec2f GetVec2f(const char *option) const final;

private:
	json config;
};
