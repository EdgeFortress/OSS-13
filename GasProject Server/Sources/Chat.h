#pragma once

#include <string>
#include <vector>

class Chat {
private:
	struct message {
		std::string text;
		bool sended;

		message(const std::string &text) : text(text), sended(false) { }
	};
	std::vector<message> messages;

public:
	Chat() = default;
	Chat(Chat &chat) = delete;
	Chat& operator=(Chat &chat) = delete;
	~Chat() = default;

	void AddMessage(const std::string &message) {
		messages.push_back(Chat::message(message));
	}

	std::vector<std::string> GetNewMessages() {
		std::vector<std::string> newMessages;

		int i = int(messages.size());
		while (--i >= 0 && !messages[i].sended);

		while (++i < int(messages.size()))
			newMessages.push_back(messages[i].text), messages[i].sended = true;

		return std::move(newMessages);
	}
};
