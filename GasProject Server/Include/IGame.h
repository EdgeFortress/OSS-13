#pragma once

#include <Shared/IFaces/INonCopyable.h>
#include <Shared/Types.hpp>

class Player;
class Control;
class World;
class Chat;
class IScriptEngine;

class IGame : public INonCopyable {
public:
	// True if new player created, false if exist player reconnected
	virtual bool AddPlayer(sptr<Player> &) = 0;
	virtual void SendChatMessages() = 0;

	virtual Control *GetStartControl(Player *) = 0;
	virtual const uptr<World> &GetWorld() const = 0;
	virtual IScriptEngine *GetScriptEngine() const = 0;
	virtual Chat *GetChat() = 0;
};

extern IGame *GGame;
