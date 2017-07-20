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
		//SHIFT,
		CHANGE_DIRECTION
	};

	enum class Sprite : char {
		EMPTY = 0,
		Floor,
		Wall,
		Airlock,
		Mob,
		Ghost,
		Space,
		NextSprite = Space + 26
	};
}