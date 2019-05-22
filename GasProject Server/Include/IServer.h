#pragma once

#include <Shared/IFaces/INonCopyable.h>
#include <Shared/Types.hpp>

class Player;
class IGame;
class ResourceManager;

class IServer : public INonCopyable {
public:
	virtual Player *Authorization(const std::string &login, const std::string &password) const = 0;
	virtual bool Registration(const std::string &login, const std::string &password) const = 0;
	virtual bool JoinGame(sptr<Player> &player) const = 0;

	virtual IGame *GetGame() const = 0;
	//virtual UsersDB *GetUDB() const;
	virtual ResourceManager *GetRM() const = 0;
};

extern IServer *GServer;
