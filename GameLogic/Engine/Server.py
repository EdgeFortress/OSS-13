from __future__ import annotations

from Engine_Server import *
from Engine.World import World, Control

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
	def world() -> World:
		return World(eGGame.world)

class VerbsHolder(eVerbsHolder):
	"""
	Inheritance from this class allow to define special actions without
	arguments which can be called with command from console

	VerbsHolder is registrated with unique name, so player can call it's verbs.

	For example, class Player registrated with name "Player".
	So player's verb "Drop" can be called with next command: "Player.Drop"

	Methods
	-------
	AddVerb(name: str, action: Callable[[], None])
		add new verb

		Parametres
		----------
		name: str
			verb's key. Use name to call verb from console

	"""

	def __init__(self, impl):
		self._impl = impl

	def AddVerb(self, name: str, action: Callable[[], None]):
		self._impl.AddVerb(name, action)

class Player(ePlayer, VerbsHolder):
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
		VerbsHolder.__init__(self, impl)

	@property
	def ckey(self) -> str:
		return self._impl.ckey

	@property
	def control(self) -> Control:
		return Control(self._impl.control)
	@control.setter
	def control(self, value):
		self._impl.control.fset(value)

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
	DEFAULT			= eItemSpriteState.DEFAULT 			"""default icon"""
	ON_MOB			= eItemSpriteState.ON_MOB			"""is being weared by creature"""
	IN_HAND_LEFT	= eItemSpriteState.IN_HAND_LEFT		"""is being holded in left hand"""
	IN_HAND_RIGHT	= eItemSpriteState.IN_HAND_RIGHT	"""is being holded in right hand"""


gServer = eGServer
gGame = Game(eGGame)
gRM = ResourceManager(eGServer.RM)
