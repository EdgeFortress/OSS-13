#pragma once

#include <string>

#include <PlayerCommand.hpp>
#include <ClientUI/WindowSink.h>
#include <World/Camera/Camera.hpp>

#include <Shared/Types.hpp>
#include <Shared/ThreadSafeQueue.hpp>
#include <Shared/Network/Protocol/InputData.h>
#include <Shared/Network/Protocol/Command.h>

#include "VerbsHolder.h"

class Server;
class Game;
class NetworkController;
struct Connection;
struct ServerCommand;

class Control;

class Player : public VerbsHolder {
friend NetworkController;
friend Server;

public:
	explicit Player(std::string ckey);

	void SetConnection(sptr<Connection> &connection);

    /// Client interface
    void JoinToGame();
    void ChatMessage(std::string &message);

    void Move(uf::Direction);
    void MoveZ(bool up);
    void ClickObject(uint id);
	void ClickControlUI(const std::string &id);

	void UIInput(uptr<network::protocol::UIData> &&data);
	void UITrigger(const std::string &window, const std::string &trigger);
	void CallVerb(const std::string &verb);
    ///

    void Update(std::chrono::microseconds timeElapsed);
    void SendGraphicsUpdates(std::chrono::microseconds timeElapsed);

	template <class T>
	void OpenWindow() {
		auto window = std::make_unique<T>(this);
		window->Initialize();
		uiSinks[window->Id()] = std::move(window);
	}
	void OpenSpawnWindow();

	const std::string &GetCKey() const { return ckey; }

	void Suspend();
    void SetControl(Control *control);
	void SetCamera(Camera *camera);

	sptr<Connection> GetConnection();
	Control *GetControl();
	Camera *GetCamera();
	bool IsConnected();

    void AddCommandToClient(network::protocol::Command *);

	void AddSyncCommandFromClient(uptr<network::protocol::Command> &&command);

private:
	void updateUISinks(std::chrono::microseconds timeElapsed);

	void spawnWindowSearchCommand(const std::string &searchBuffer);
	void spawnWindowSpawnCommand(const std::string &typeKey);

private:
	std::string ckey;

	Control *control;
	uptr<Camera> camera;

	wptr<Connection> connection;
	uf::ThreadSafeQueue<PlayerCommand *> actions;
	uf::ThreadSafeQueue<uptr<network::protocol::Command>> syncCommands;

	bool atmosOverlayToggled;

	std::map<std::string, uptr<WindowSink>> uiSinks;
	std::map<std::string, const IVerbsHolder *> verbsHolders;
};
