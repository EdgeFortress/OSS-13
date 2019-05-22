#include "Archive.h"

#include "ISerializable.h"

#include <Shared/Network/Protocol/ClientCommand.h>
#include <Shared/Network/Protocol/OverlayInfo.h>
#include <Shared/Network/Protocol/InputData.h>
#include <Shared/Network/Protocol/WindowData.h>

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
		DECLARE_SER(OverlayInfo)
		DECLARE_SER(RadioButtonUIData)
		DECLARE_SER(WindowData)

		// Client Commands
		DECLARE_SER(AuthorizationClientCommand)
		DECLARE_SER(RegistrationClientCommand)
		DECLARE_SER(GamelistRequestClientCommand)
		DECLARE_SER(JoinGameClientCommand)
		DECLARE_SER(MoveClientCommand)
		DECLARE_SER(MoveZClientCommand)
		DECLARE_SER(ClickObjectClientCommand)
		DECLARE_SER(DropClientCommand)
		DECLARE_SER(SendChatMessageClientCommand)
		DECLARE_SER(BuildClientCommand)
		DECLARE_SER(GhostClientCommand)
		DECLARE_SER(UIInputClientCommand)
		DECLARE_SER(UITriggerClientCommand)
		DECLARE_SER(CallVerbClientCommand)
		DECLARE_SER(DisconnectionClientCommand)

		default:
			throw std::exception(); // unknown id
	}
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
