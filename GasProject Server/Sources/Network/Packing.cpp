#include "NetworkController.hpp"
#include "Common/Ñommand.hpp"

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
                packet << sf::Int32(command->firstBlockX) << sf::Int32(command->firstBlockY);
                packet << sf::Int32(command->blocksInfo.size());
                for (auto &blockInfo : command->blocksInfo)
                    packet << blockInfo;
            }
            if (command->options & GraphicsUpdateServerCommand::Option::CAMERA_MOVE) {
                packet << sf::Int32(command->cameraX) << sf::Int32(command->cameraY);
            }
            if (command->options & GraphicsUpdateServerCommand::Option::DIFFERENCES) {
                packet << sf::Int32(command->diffs.size());
                for (auto &diff : command->diffs)
                    packet << *(diff);
            }
            break;
        }
        case ServerCommand::Code::SEND_CHAT_MESSAGE: {
            auto c = dynamic_cast<SendChatMessageServerCommand *>(serverCommand);
            packet << c->playerName;
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
    if (diff.GetType() == Diff::Type::NONE) return packet;
    packet << Int32(diff.GetType());
    packet << Int32(diff.block->X());
    packet << Int32(diff.block->Y());
    packet << Int32(diff.x) << Int32(diff.y);
    packet << Int32(diff.objectNum);
    switch (diff.GetType()) {
        case Diff::Type::MOVE: {
            const MoveDiff &moveDiff = dynamic_cast<const MoveDiff &>(diff);
            packet << Int32(moveDiff.toX) << Int32(moveDiff.toY) << Int32(moveDiff.toObjectNum);
            break;
        }
        case Diff::Type::ADD: {
            const AddDiff &addDiff = dynamic_cast<const AddDiff &>(diff);
            packet << Int32(addDiff.sprite);
            packet << String(addDiff.name);
            break;
        }
        case Diff::Type::REMOVE: {
            const RemoveDiff &removeDiff = dynamic_cast<const RemoveDiff &>(diff);
            break;
        }
    }
    return packet;
}

Packet &operator<<(Packet &packet, const BlockInfo &blockInfo) {
    packet << sf::Int32(blockInfo.x) << sf::Int32(blockInfo.y);
    for (auto &tileInfo : blockInfo.tiles)
        packet << tileInfo;
    return packet;
}

Packet &operator<<(Packet &packet, const TileInfo &tileInfo) {
    packet << sf::Int32(tileInfo.content.size()) << sf::Int32(tileInfo.sprite);
    for (auto &objInfo : tileInfo.content) {
        packet << objInfo;
    }
    return packet;
}

Packet &operator<<(Packet &packet, const ObjectInfo &objInfo) {
    packet << sf::Int32(objInfo.sprite) << sf::String(objInfo.name);
    return packet;
}