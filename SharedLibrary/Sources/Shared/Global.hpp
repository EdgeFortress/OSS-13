#pragma once

namespace Global {
	const int PORT = 55678;

	const int BLOCK_SIZE = 8;
	const int FOV = 15; // Field Of View
	const int MIN_PADDING = 3;

	enum class DiffType : char {
		NONE = 0,
		ADD,
		REMOVE,
		RELOCATE,
        MOVE_INTENT,
		MOVE,
		CHANGE_SPRITE,
        PLAY_ANIMATION,
		CHANGE_DIRECTION
	};

	const uint32_t MAX_SPRITE_ID = 0x0FFFFFFFu; // (= 268 435 455) 4 bites reserved for state

	enum class ItemSpriteState : uint32_t {
		DEFAULT    = 0,          // on the ground
		// next states applies to Items only for now
		ON_MOB        = 0x10000000, // on mob
		IN_HAND_LEFT  = 0x20000000, // in left hand
		IN_HAND_RIGHT = 0x30000000, // in right hand
		// ...
		MASK = 0xF0000000
	};
}
