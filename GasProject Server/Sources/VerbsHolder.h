#pragma once

#include <functional>
#include <string>
#include <map>

class Player;

using Verb = std::function<void(Player *)>;

class IVerbsHolder {
public:
	virtual void AddVerb(std::string key, Verb action) = 0;
	virtual const std::map<std::string, Verb> &GetVerbs() const = 0;

	virtual ~IVerbsHolder() = default;
};

class VerbsHolder : public IVerbsHolder {
public:
	void AddVerb(std::string key, Verb action) final {
		verbs[key] = action;
	}

	const std::map<std::string, Verb> &GetVerbs() const final {
		return verbs;
	}

private:
	std::map<std::string, Verb> verbs;
};