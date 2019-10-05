#include "ISerializable.h"

#include "Archive.h"

#include <Shared/Network/Protocol/InputData.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>
#include <Shared/Network/Protocol/ServerToClient/OverlayInfo.h>
#include <Shared/Network/Protocol/ServerToClient/WindowData.h>
#include <Shared/Network/Protocol/ServerToClient/Diff.h>

using namespace network::protocol;

namespace uf {

void ISerializable::Serialize(Archive &archive) {
	if (archive.GetMode() == Archive::Mode::Input)
		archive << sf::Int32(SerID());
}

#define DECLARE_SER(name) \
	case #name##_crc32: { return std::make_unique<name>(); }

std::unique_ptr<ISerializable> CreateSerializableById(uint32_t id) {
	switch (id) {
		using namespace client;
		DECLARE_SER(AuthorizationCommand)
		DECLARE_SER(RegistrationCommand)
		DECLARE_SER(GamelistRequestCommand)
		DECLARE_SER(JoinGameCommand)
		DECLARE_SER(MoveCommand)
		DECLARE_SER(MoveZCommand)
		DECLARE_SER(ClickObjectCommand)
		DECLARE_SER(ClickTileCommand)
		DECLARE_SER(ClickControlUICommand)
		DECLARE_SER(SendChatMessageCommand)
		DECLARE_SER(UIInputCommand)
		DECLARE_SER(UITriggerCommand)
		DECLARE_SER(SpawnWindowSearchCommand)
		DECLARE_SER(SpawnWindowSpawnCommand)
		DECLARE_SER(ContextMenuUpdateCommand)
		DECLARE_SER(ContextMenuClickCommand)
		DECLARE_SER(CallVerbCommand)
		DECLARE_SER(DisconnectionCommand)
	}

	switch (id) {
		using namespace server;
		DECLARE_SER(AuthorizationSuccessCommand)
		DECLARE_SER(AuthorizationFailedCommand)
		DECLARE_SER(RegistrationSuccessCommand)
		DECLARE_SER(RegistrationFailedCommand)
		DECLARE_SER(GameJoinSuccessCommand)
		DECLARE_SER(GameJoinErrorCommand)
		DECLARE_SER(GraphicsUpdateCommand)
		DECLARE_SER(ControlUIUpdateCommand)
		DECLARE_SER(OverlayUpdateCommand)
		DECLARE_SER(OverlayResetCommand)
		DECLARE_SER(OpenWindowCommand)
		DECLARE_SER(OpenSpawnWindowCommand)
		DECLARE_SER(UpdateSpawnWindowCommand)
		DECLARE_SER(UpdateWindowCommand)
		DECLARE_SER(UpdateContextMenuCommand)
		DECLARE_SER(AddChatMessageCommand)
	}

	switch (id) {
		DECLARE_SER(RadioButtonUIData)
		DECLARE_SER(OverlayInfo)
		DECLARE_SER(WindowData)
		DECLARE_SER(ContextMenuData)
		DECLARE_SER(ContextMenuNode)
	}

	switch (id) {
		DECLARE_SER(RelocateDiff)
		DECLARE_SER(RelocateAwayDiff)
		DECLARE_SER(AddDiff)
		DECLARE_SER(RemoveDiff)
		DECLARE_SER(FieldsDiff)
		DECLARE_SER(MoveIntentDiff)
		DECLARE_SER(MoveDiff)
		DECLARE_SER(UpdateIconsDiff)
		DECLARE_SER(PlayAnimationDiff)
		DECLARE_SER(StunnedDiff)
	}

	EXPECT_WITH_MSG(false, "Unknown serializable id: " + std::to_string(id));
}

} // namespace uf
