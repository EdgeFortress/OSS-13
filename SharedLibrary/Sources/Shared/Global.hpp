#pragma once

namespace Global {
	const int PORT = 55700;

	const int FOV = 15; // Field Of View
	const int MIN_PADDING = 3;
	const int Z_FOV = 1;

	enum class DiffType : char {
		NONE = 0,
		ADD,
		REMOVE,
		RELOCATE,
        MOVE_INTENT,
		MOVE,
		UPDATE_ICONS,
        PLAY_ANIMATION,
		CHANGE_DIRECTION,
		STUNNED,
		OVERLAY_UPDATE
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
