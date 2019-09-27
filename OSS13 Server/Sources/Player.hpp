#pragma once

#include <string>

#include <ClientUI/WindowSink.h>
#include <World/Camera/Camera.hpp>

#include <Shared/Types.hpp>
#include <Shared/ThreadSafeQueue.hpp>
#include <Shared/Network/Protocol/InputData.h>
#include <Shared/Network/Protocol/Command.h>
#include <Shared/Network/Protocol/CommandsProcessor.h>

#include "VerbsHolder.h"

struct Connection;
class PlayerCommandsProcessor;
class Control;

class Player : public VerbsHolder {
friend PlayerCommandsProcessor;

public:
	explicit Player(std::string ckey);

	void SetConnection(sptr<Connection> &connection);

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

private:
	std::string ckey;
	bool justJoined{true};

	Control *control;
	uptr<Camera> camera;

	wptr<Connection> connection;
	uptr<network::CommandsProcessor> playerCommandsProcessor;

	bool atmosOverlayToggled;

	std::map<std::string, uptr<WindowSink>> uiSinks;
	std::map<std::string, const IVerbsHolder *> verbsHolders;
};
