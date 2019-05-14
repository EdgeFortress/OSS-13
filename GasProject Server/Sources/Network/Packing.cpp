#include "NetworkController.hpp"

#include <Server.hpp>
#include <World/World.hpp>

#include <Shared/Network/Archive.h>

#include "Differences.hpp"

using namespace sf;

Packet &operator<<(Packet &packet, ServerCommand *serverCommand) {
    ServerCommand::Code code = serverCommand->GetCode();
    packet << sf::Int32(code);
    switch (code) {
        case ServerCommand::Code::GAME_LIST: {
            packet << sf::Int32(Server::Get()->GetGamesList()->size());
            for (auto &game : *(Server::Get()->GetGamesList())) {
                packet << *(game.get());
            }
            break;
        }
        case ServerCommand::Code::GRAPHICS_UPDATE: {
            GraphicsUpdateServerCommand *command = dynamic_cast<GraphicsUpdateServerCommand *>(serverCommand);
            packet << sf::Int32(command->options);
            if (command->options & GraphicsUpdateServerCommand::Option::BLOCKS_SHIFT) {
                packet << sf::Int32(command->firstBlockX) << sf::Int32(command->firstBlockY) << sf::Int32(command->firstBlockZ);
                packet << sf::Int32(command->blocksInfo.size());
                for (auto &blockInfo : command->blocksInfo)
                    packet << blockInfo;
            }
            if (command->options & GraphicsUpdateServerCommand::Option::CAMERA_MOVE) {
                packet << sf::Int32(command->cameraX) << sf::Int32(command->cameraY) << sf::Int32(command->cameraZ);
            }
            if (command->options & GraphicsUpdateServerCommand::Option::DIFFERENCES) {
                packet << sf::Int32(command->diffs.size());
				for (auto &diff : command->diffs) {
					packet << *(diff);
				}
            }
            if (command->options & GraphicsUpdateServerCommand::Option::NEW_CONTROLLABLE) {
                packet << command->controllable_id << command->controllableSpeed;
            }
            break;
        }
		case ServerCommand::Code::OVERLAY_UPDATE:
		{
			auto command = dynamic_cast<OverlayUpdateServerCommand *>(serverCommand);
			for (auto &overlayTileInfo : command->overlayInfo) {
				uf::InputArchive r(packet);
				overlayTileInfo.Serialize(r);
			}
			break;
		}
		case ServerCommand::Code::OPEN_WINDOW:
		{
			auto c = dynamic_cast<OpenWindowServerCommand *>(serverCommand);
			packet << c->layout;
			break;
		}
        case ServerCommand::Code::SEND_CHAT_MESSAGE: {
            auto c = dynamic_cast<SendChatMessageServerCommand *>(serverCommand);
            packet << c->message;
            break;
        }
    }

    return packet;
}

Packet &operator<<(Packet &packet, Game &game) {
    packet << sf::Int32(game.id);
    packet << sf::String(game.title);
    packet << sf::Int32(game.players.size());
    return packet;
}

Packet &operator<<(Packet &packet, const Diff &diff) {
    if (diff.GetType() == Global::DiffType::NONE) return packet;
    packet << Int32(diff.GetType());
    switch (diff.GetType()) {
        case Global::DiffType::RELOCATE: {
            packet << Int32(diff.id);
            const ReplaceDiff &moveDiff = dynamic_cast<const ReplaceDiff &>(diff);
            packet << Int32(moveDiff.toX) << Int32(moveDiff.toY) << Int32(moveDiff.toZ) << Int32(moveDiff.toObjectNum);
            break;
        }
        case Global::DiffType::ADD: {
            const AddDiff &addDiff = dynamic_cast<const AddDiff &>(diff);
            packet << addDiff.objectInfo;
            break;
        }
        case Global::DiffType::REMOVE: {
            packet << Int32(diff.id);
            const RemoveDiff &removeDiff = dynamic_cast<const RemoveDiff &>(diff);
            break;
        }
        case Global::DiffType::MOVE_INTENT: {
            packet << Int32(diff.id);
            const MoveIntentDiff &moveIntentDiff = dynamic_cast<const MoveIntentDiff &>(diff);
            packet << Int8(moveIntentDiff.direction);
            break;
        }
        case Global::DiffType::MOVE: {
            packet << Int32(diff.id);
            const MoveDiff &moveDiff = dynamic_cast<const MoveDiff &>(diff);
            packet << Int8(moveDiff.direction);
			packet << moveDiff.speed;
            break;
        }
		case Global::DiffType::UPDATE_ICONS: {
			packet << Int32(diff.id);
			const UpdateIconsDiff &changeSpriteDiff = dynamic_cast<const UpdateIconsDiff &>(diff);
			packet << sf::Int32(changeSpriteDiff.icons.size());
			for (auto &iconInfo : changeSpriteDiff.icons)
				packet << sf::Int32(iconInfo.id + static_cast<uint32_t>(iconInfo.state));
			break;
		}
        case Global::DiffType::PLAY_ANIMATION:
        {
            packet << Int32(diff.id);
            const PlayAnimationDiff &changeSpriteDiff = dynamic_cast<const PlayAnimationDiff &>(diff);
            packet << Int32(changeSpriteDiff.animation_id);
            break;
        }
		case Global::DiffType::CHANGE_DIRECTION: {
			packet << Int32(diff.id);
			const ChangeDirectionDiff &changeDirectionDiff = dynamic_cast<const ChangeDirectionDiff &>(diff);
			packet << Int8(changeDirectionDiff.direction);
			break;
		}
		case Global::DiffType::STUNNED:
		{
			packet << Int32(diff.id);
			const StunnedDiff &stunnedDiff = dynamic_cast<const StunnedDiff &>(diff);
			packet << stunnedDiff.duration.asMilliseconds();
			break;
		}
    }
    return packet;
}

Packet &operator<<(Packet &packet, const TileInfo &tileInfo) {
    packet << sf::Int32(tileInfo.x) << sf::Int32(tileInfo.y) << sf::Int32(tileInfo.z);
    packet << sf::Int32(tileInfo.content.size()) << sf::Int32(tileInfo.sprite);
    for (auto &objInfo : tileInfo.content) {
        packet << objInfo;
    }
    return packet;
}

Packet &operator<<(Packet &packet, const ObjectInfo &objInfo) {
    packet << sf::Int32(objInfo.id);

	packet << sf::Int32(objInfo.spriteIds.size());
	for (auto &sprite: objInfo.spriteIds)
		packet << sf::Int32(sprite);

	packet << sf::String(objInfo.name) << sf::Int32(objInfo.layer);
	packet << sf::Int8(objInfo.direction);
	packet << objInfo.dense;
    packet << objInfo.moveSpeed;
    packet << objInfo.constSpeed.x << objInfo.constSpeed.y;
    return packet;
}