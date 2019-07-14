#include "Archive.h"

#include "ISerializable.h"

#include <Shared/ErrorHandling.h>

#include <Shared/Network/Protocol/InputData.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>
#include <Shared/Network/Protocol/ServerToClient/OverlayInfo.h>
#include <Shared/Network/Protocol/ServerToClient/WindowData.h>

using namespace uf;
using namespace network::protocol;

// Archive

Archive::Archive(sf::Packet &packet) :
	packet(packet)
{ }

#define DECLARE_SER(name) \
	case #name##_crc32: { uptr<ISerializable> p = std::make_unique<name>(); p->Serialize(*this); return p; }

uptr<ISerializable> Archive::UnpackSerializable() {
	sf::Int32 id;
	packet >> id;

	switch (id) {
		using namespace client;
		DECLARE_SER(AuthorizationCommand)
		DECLARE_SER(RegistrationCommand)
		DECLARE_SER(GamelistRequestCommand)
		DECLARE_SER(JoinGameCommand)
		DECLARE_SER(MoveCommand)
		DECLARE_SER(MoveZCommand)
		DECLARE_SER(ClickObjectCommand)
		DECLARE_SER(SendChatMessageCommand)
		DECLARE_SER(UIInputCommand)
		DECLARE_SER(UITriggerCommand)
		DECLARE_SER(CallVerbCommand)
		DECLARE_SER(DisconnectionCommand)
	}

	switch (id) {
		DECLARE_SER(RadioButtonUIData)
		DECLARE_SER(OverlayInfo)
		DECLARE_SER(WindowData)
	}

	EXPECT_WITH_MSG(false, "Unknown id received: " + std::to_string(id));
}


// InputArchive

InputArchive::InputArchive(sf::Packet &packet) :
	Archive(packet)
{ 
	isOut = false;
}

// OutputArchive

OutputArchive::OutputArchive(sf::Packet &packet) :
	Archive(packet)
{ 
	isOut = true;
}
