from __future__ import annotations

from Engine_Server import *
from Engine.World import World, Control

from enum import Enum
from typing import Callable

class Game(eGame):
	def __init__(self, impl):
		self._impl = impl

	@property
	def world() -> World:
		return World(eGGame.world)

class VerbsHolder(eVerbsHolder):
	def __init__(self, impl):
		self._impl = impl

	def AddVerb(self, name: str, action: Callable[[], None]):
		self._impl.AddVerb(name, action)

class Player(ePlayer, VerbsHolder):
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
	def __init__(self, impl):
		self._impl = impl

	def GetIcon(self, title: str, state: ItemSpriteState) -> eIcon:
		return self._impl.GetIcon(title, state)


gServer = eGServer
gGame = Game(eGGame)
gRM = ResourceManager(eGServer.RM)


class ItemSpriteState(eItemSpriteState):
	DEFAULT			= eItemSpriteState.DEFAULT
	ON_MOB			= eItemSpriteState.ON_MOB
	IN_HAND_LEFT	= eItemSpriteState.IN_HAND_LEFT
	IN_HAND_RIGHT	= eItemSpriteState.IN_HAND_RIGHT
