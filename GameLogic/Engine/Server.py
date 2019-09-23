from __future__ import annotations

from Engine_Server import *

import Engine.World

from enum import Enum
from typing import Callable

class Game(eGame):
	"""
	Game object.

	Note: Use gGame global instance!

	Attributes
	----------
	world: World
		Game's world
		Read-only

	"""

	def __init__(self, impl):
		self._impl = impl

	@property
	def world() -> Engine.World.World:
		return Engine.World.World(eGGame.world)

class Player(ePlayer, Engine.World.VerbsHolder):
	"""
	Player class is persistent until restart. When player re-logins, existed Player instance is used.

	Attributes
	----------
	ckey: str
		client key, nickname
		Read-only

	control: Control
		control component used by player for creature controlling


	Methods
	-------
	IsConnected() -> bool
		returns True when player is online

	"""

	def __init__(self, impl):
		Engine.World.VerbsHolder.__init__(self, impl)

	@property
	def ckey(self) -> str:
		return self._impl.ckey

	@property
	def control(self) -> Engine.World.Control:
		return Engine.World.Control(self._impl.control)
	@control.setter
	def control(self, value: Engine.World.Control):
		self._impl.control = value._impl

	def IsConnected(self) -> bool:
		return self._impl.IsConnected()


class ResourceManager(eResourceManager):
	"""
	Class for resources management

	Note: Use gRM global instance!

	Methods
	-------
	GetIcon(title: str, state: ItemSpriteState) -> eIcon
		returns icon object by it's string id and state

	"""

	def __init__(self, impl):
		self._impl = impl

	def GetIcon(self, title: str, state: ItemSpriteState) -> eIcon:
		return self._impl.GetIcon(title, state)


class ItemSpriteState(eItemSpriteState):
	""" Enumeration of icon states"""

	# default icon
	DEFAULT			= eItemSpriteState.DEFAULT

	# is being weared by creature
	ON_MOB			= eItemSpriteState.ON_MOB

	# is being holded in left hand
	IN_HAND_LEFT	= eItemSpriteState.IN_HAND_LEFT

	# is being holded in right hand
	IN_HAND_RIGHT	= eItemSpriteState.IN_HAND_RIGHT


gServer = eGServer
gGame = Game(eGGame)
gRM = ResourceManager(eGServer.RM)
