#pragma once

#include <Shared/Network/Protocol/Command.h>
#include <Shared/Network/Protocol/InputData.h>
#include <Shared/Network/ArchiveConverters.h>

namespace network {
namespace protocol {
namespace client {

DEFINE_SERIALIZABLE(AuthorizationCommand, Command)
	std::string login;
	std::string password;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & login;
		ar & password;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(RegistrationCommand, Command)
	std::string login;
	std::string password;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & login;
		ar & password;
	}
DEFINE_SERIALIZABLE_END

DEFINE_PURE_SERIALIZABLE(GamelistRequestCommand, Command)

DEFINE_SERIALIZABLE(JoinGameCommand, Command)
	int id;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & id;
	}
DEFINE_SERIALIZABLE_END

DEFINE_PURE_SERIALIZABLE(DisconnectionCommand, Command)

DEFINE_SERIALIZABLE(MoveCommand, Command)
	uf::Direction direction;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & direction;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(MoveZCommand, Command)
	bool up;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & up;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(ClickObjectCommand, Command)
	int id;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & id;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(ClickControlUICommand, Command)
	std::string id; // PERF: can be optimized (int id)
	
	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & id;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(SendChatMessageCommand, Command)
	std::string message;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & message;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(UIInputCommand, Command)
	std::string handle;
	uptr<UIData> data;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & handle;
		ar & data;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(UITriggerCommand, Command)
	std::string window;
	std::string trigger;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & window;
		ar & trigger;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(CallVerbCommand, Command)
	std::string verb;

	void Serialize(uf::Archive &ar) override {
		Command::Serialize(ar);
		ar & verb;
	}
DEFINE_SERIALIZABLE_END

} // namespace client
} // namespace protocol
} // namespace network
