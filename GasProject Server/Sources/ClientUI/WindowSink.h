#pragma once

#include <string>
#include <map>
#include <chrono>

#include <Shared/Network/Protocol/InputData.h>

class Player;

class WindowSink {
public:
	WindowSink(Player *player, const std::string &id);
	virtual ~WindowSink() = default;
	void Initialize();

	virtual void Update(std::chrono::microseconds timeElapsed);

	virtual void OnTrigger(const std::string &trigger);
	virtual void OnInput(const std::string &input, uptr<network::protocol::UIData> &&data);

	const std::string &Id() ;
	Player *GetPlayer();
	bool IsClosed();

protected:
	std::map<std::string, uptr<network::protocol::UIData>> fields;

private:
	Player *player;
	const std::string id;

	bool isClosed;
};
