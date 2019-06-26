#pragma once

#include <IVerbsHolder.h>

class VerbsHolder : public IVerbsHolder {
public:
	void AddVerb(const std::string &name, Verb action) final;
	void CallVerb(Player *player, const std::string &name) const final;
	const std::map<std::string, Verb> &GetVerbs() const final;

private:
	std::map<std::string, Verb> verbs;
};