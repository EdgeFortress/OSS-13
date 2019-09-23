#include "VerbsHolder.h"

#include <Shared/ErrorHandling.h>

using namespace std::string_literals;

void VerbsHolder::AddVerb(const std::string &name, Verb action) {
	verbs[name] = action;
}

void VerbsHolder::CallVerb(Player *player, const std::string &name) const {
	try {
		auto nameAndVerb = verbs.find(name);
		EXPECT_WITH_MSG(nameAndVerb != verbs.end(), "Verb \""s + name + "\" doesn't exist!");
		nameAndVerb->second(player);
	} catch (const std::exception &e) {
		MANAGE_EXCEPTION(e);
	}
}

const std::map<std::string, Verb> &VerbsHolder::GetVerbs() const {
	return verbs;
}