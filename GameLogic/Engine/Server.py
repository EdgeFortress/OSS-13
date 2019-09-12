from Engine_Server import *
from Engine.World import World, Control

from enum import Enum

class Game(eGame):
	def __new__(cls, parent):
		return parent

	@property
	def world() -> World:
		return World(eGGame.world)


class Player(ePlayer):
	def __new__(cls, parent):
		return parent

	@property
	def ckey(self) -> str:
		return super().ckey

	@property
	def control(self) -> Control:
		return Control(super().control)

	@control.setter
	def control(self, value):
		super(Player, self.__class__).control.fset(self, value)

	def IsConnected(self) -> bool:
		return super().IsConnected()


class ResourceManager(eResourceManager):
	def __new__(cls, parent):
		return parent

	def GetIcon(title, state) -> eIcon:
		eGServer.RM.GetIcon(title, state)


gServer = eGServer
gGame = Game(eGGame)
gRM = ResourceManager(eGServer.RM)


class ItemSpriteState(eItemSpriteState):
	DEFAULT			= eItemSpriteState.DEFAULT
	ON_MOB			= eItemSpriteState.ON_MOB
	IN_HAND_LEFT	= eItemSpriteState.IN_HAND_LEFT
	IN_HAND_RIGHT	= eItemSpriteState.IN_HAND_RIGHT
