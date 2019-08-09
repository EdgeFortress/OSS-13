#pragma once

#include <string>
#include <map>
#include <functional>

class Player;

using Verb = std::function<void(Player *)>;

class IVerbsHolder {
public:
	virtual void AddVerb(const std::string &name, Verb action) = 0;
	virtual void CallVerb(Player *player, const std::string &name) const = 0;
	virtual const std::map<std::string, Verb> &GetVerbs() const = 0;

	virtual ~IVerbsHolder() = default;
};