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
		MOVE,
		CHANGE_SPRITE,
        PLAY_ANIMATION,
		CHANGE_DIRECTION
	};
}