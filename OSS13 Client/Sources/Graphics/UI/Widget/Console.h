#pragma once

#include <map>
#include <functional>

#include <Graphics/UI/Widget/ImGuiWidget.h>

class Console : public ImGuiWidget {
public:
	Console();

	virtual void Update(sf::Time timeElapsed) final;

private:
	void loadMessages();

	std::string lowerString(const std::string&);

	void command_help();
	void command_clear();
	void command_notFound(const std::string&);

private:
	std::vector<std::string> rows;
	std::map<std::string, std::function<void()>> commands;
	std::string buffer;

	bool scrollToBottom;
};
