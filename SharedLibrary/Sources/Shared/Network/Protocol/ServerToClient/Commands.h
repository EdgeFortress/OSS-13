#pragma once

#include <Shared/Network/Protocol/Command.h>
#include <Shared/Network/Protocol/ServerToClient/ControlUIData.h>
#include <Shared/Network/Protocol/ServerToClient/OverlayInfo.h>
#include <Shared/Network/Protocol/ServerToClient/WindowData.h>
#include <Shared/Network/Protocol/ServerToClient/WorldInfo.h>
#include <Shared/Network/Protocol/ServerToClient/Diff.h>

namespace network {
namespace protocol {
namespace server {

DEFINE_PURE_SERIALIZABLE(AuthorizationSuccessCommand, Command)

DEFINE_PURE_SERIALIZABLE(RegistrationSuccessCommand, Command)

DEFINE_PURE_SERIALIZABLE(AuthorizationFailedCommand, Command)

DEFINE_PURE_SERIALIZABLE(RegistrationFailedCommand, Command)

DEFINE_PURE_SERIALIZABLE(GameCreateSuccessCommand, Command)

DEFINE_PURE_SERIALIZABLE(GameCreateErrorCommand, Command)

DEFINE_PURE_SERIALIZABLE(GameJoinSuccessCommand, Command)

DEFINE_PURE_SERIALIZABLE(GameJoinErrorCommand, Command)

DEFINE_SERIALIZABLE(GraphicsUpdateCommand, Command)
	enum Option : char {
		EMPTY = 0,
		TILES_SHIFT = 1,
		CAMERA_MOVE = 1 << 1,
		DIFFERENCES = 1 << 2,
		NEW_CONTROLLABLE = 1 << 3
	};

	sf::Int8 options;

	std::vector<std::shared_ptr<Diff>> diffs;
	std::vector<TileInfo> tilesInfo;

	uf::vec3i camera;
	uf::vec3i firstTile;
	int controllableId;
	float controllableSpeed;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);

		ar & options;
		
		if (options & TILES_SHIFT) {
			ar & firstTile;
			ar & tilesInfo;
		}
		if (options & CAMERA_MOVE) {
			ar & camera;
		}
		if (options & DIFFERENCES) {
			ar & diffs;
		}
		if (options & NEW_CONTROLLABLE) {
			ar & controllableId;
			ar & controllableSpeed;
		}
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(ControlUIUpdateCommand, Command)
	bool clear{false};
	std::vector<network::protocol::ControlUIData> elements;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & clear;
		ar & elements;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(OverlayUpdateCommand, Command)
	std::vector<network::protocol::OverlayInfo> overlayInfo;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & overlayInfo;
	}
DEFINE_SERIALIZABLE_END

DEFINE_PURE_SERIALIZABLE(OverlayResetCommand, Command)

DEFINE_SERIALIZABLE(OpenWindowCommand, Command)
	std::string id;
	network::protocol::WindowData data;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & id;
		ar & data;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(UpdateWindowCommand, Command)
	uptr<network::protocol::UIData> data;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & data;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(AddChatMessageCommand, Command)
	std::string message;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & message;
	}
DEFINE_SERIALIZABLE_END

} // namespace server
} // namespace protocol
} // namespace network
