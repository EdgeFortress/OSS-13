from __future__ import annotations

from Engine_World import *
from Engine_Geometry import *

from datetime import timedelta
from typing import Callable

class World(eWorld):
	def __new__(cls, parent):
		return parent

	def GetObjectById(self, id: int) -> Object:
		return super().GetObjectById(id)

	def GetMap(self) -> Map:
		return super().GetMap()


class Map(eMap):
	def __new__(cls, parent):
		return parent

	@property
	def size(self) -> Vec3i:
		return super().size()

	def GetTile(pos: Vec3i) -> Tile:
		return Tile(super().GetTile(pos))


class Tile(eTile):
	def __new__(cls, parent):
		return parent

	@property
	def x(self) -> int:
		return super().x
	@property
	def y(self) -> int:
		return super().y
	@property
	def z(self) -> int:
		return super().z

	@property
	def pos(self) -> Vec3i:
		return super().pos

	@property
	def map(self) -> Map:
		return Map(super().map)

	def IsDense(self, directions: DirectionSet) -> bool:
		return super().IsDense(directions)

	def IsSpace(self) -> bool:
		return super().IsSpace()

	def GetDenseObject(self, directions: DirectionSet) -> Object:
		return Object(super().GetDenseObject(directions))


class Object(eObject):
	def __init__(self):
		super().__init__()

	@property
	def name(self) -> str:
		return super().name
	@name.setter
	def name(self, value: str):
		super(Object, self.__class__).name.fset(self, value)

	@property
	def sprite(self) -> str:
		return super().sprite
	@sprite.setter
	def sprite(self, value):
		super(Object, self.__class__).sprite.fset(self, value)

	@property
	def layer(self) -> int:
		return super().layer
	@layer.setter
	def layer(self, value: int):
		super(Object, self.__class__).layer.fset(self, value)

	@property
	def density(self) -> bool:
		return super().density
	@density.setter
	def density(self, value: bool):
		super(Object, self.__class__).density.fset(self, value)

	@property
	def solidity(self) -> DirectionSet:
		return super().solidity
	@solidity.setter
	def solidity(self, value: DirectionSet):
		super(Object, self.__class__).solidity.fset(self, value)

	@property
	def invisibility(self) -> int:
		return super().invisibility
	@invisibility.setter
	def invisibility(self, value: int):
		super(Object, self.__class__).invisibility.fset(self, value)

	@property
	def tile(self) -> Tile:
		return Tile(super().tile)
	@tile.setter
	def tile(self, value: Tile):
		super(Object, self.__class__).tile.fset(self, value)

	@property
	def position(self) -> Vec2i:
		return super().position
	@position.setter
	def position(self, value: Vec2i):
		super(Object, self.__class__).position.fset(self, value)

	@property
	def speed(self) -> Vec2f:
		return super().speed
	@speed.setter
	def speed(self, value: Vec2f):
		super(Object, self.__class__).speed.fset(self, value)

	@property
	def moveSpeed(self) -> float:
		return super().moveSpeed
	@moveSpeed.setter
	def moveSpeed(self, value: float):
		super(Object, self.__class__).moveSpeed.fset(self, value)

	@property
	def isFloor(self) -> bool:
		return super().isFloor
	@isFloor.setter
	def isFloor(self, value: bool):
		super(Object, self.__class__).isFloor.fset(self, value)

	@property
	def isWall(self) -> bool:
		return super().isWall
	@isWall.setter
	def isWall(self, value: bool):
		super(Object, self.__class__).isWall.fset(self, value)

	def Update(self, timeElapsed: timedelta):
		pass

	def InteractedBy(self, obj: Object) -> bool:
		return False

	def BumpedTo(self, obj: Object) -> bool:
		print(self.name + " bumped to " + obj.name)
		return False

	def IsCloseTo(self, obj: Object) -> bool:
		return super().IsCloseTo(obj)

	def Move(self, dir: Vec2i):
		super().Move(dir)

	def MoveZ(self, dir: int):
		super().MoveZ(dir)

	def AddComponent(self, type: str):
		super().AddComponent(type)

	def GetComponent(self, type: str) -> Component:
		return Component(super().GetComponent(type))

	def AddObject(self, obj: Object):
		super().AddObject(obj)

	def RemoveObject(self, obj: Object) -> bool:
		return super().RemoveObject(obj)

	def SetSpriteState(self, state: ItemSpriteState):
		super().SetSpriteState(state)

	def PlayAnimation(self, id: str, callback: Callable[[], None] = None) -> bool:
		return super().PlayAnimation(id, callback)

	def Delete(self):
		super().Delete()

	def _updateIcons(self):
		super()._updateIcons()

	def _pushToIcons(self, icon):
		super()._pushToIcons(icon)


def CreateObject(type: str, tile: Tile) -> Object:
	return eCreateObject(type, tile)


class Component(eComponent):
	def __new__(cls, parent):
		return parent

	def Update(self, timeElapsed: timedelta):
		super().Update(timedelta)

	def GetOwner() -> Object:
		return Object(super().GetOwner())


class Control(eControl, Component):
	def __new__(cls, parent):
		return parent

	@property
	def seeInvisibleAbility(self) -> int:
		return super().seeInvisibleAbility
	@seeInvisibleAbility.setter
	def seeInvisibleAbility(self, value: int):
		super(Control, self.__class__).seeInvisibleAbility.fset(self, value)

	def GetAndDropMoveOrder() -> Vec2i:
		return super().GetAndDropMoveOrder()

	def GetAndDropMoveZOrder() -> int:
		return super().GetAndDropMoveZOrder()

	def GetAndDropClickedObject() -> Object:
		return Object(super().GetAndDropClickedObject())


class ControlUI(eControlUI):
	def __new__(cls, parent):
		return parent

	@property
	def resolution(self) -> Vec2i:
		return super().resolution

	@property
	def center(self) -> Vec2i:
		return super().center

	@property
	def iconSize(self) -> Vec2i:
		return super().iconSize


	def UpdateElement(self, element: ControlUIElement):
		super().UpdateElement(element)

	def RemoveElement(self, elementId: str):
		super().RemoveElement(elementId)


class ControlUIElement(eControlUIElement):
	def __init__(self):
		super().__init__()

	@property
	def id(self) -> str:
		return super().id
	@id.setter
	def id(self, value: str):
		super(ControlUIElement, self.__class__).id.fset(self, value)

	@property
	def position(self) -> Vec2i:
		return super().position
	@position.setter
	def position(self, value: Vec2i):
		super(ControlUIElement, self.__class__).position.fset(self, value)

	def RegisterCallback(self, action: Callable[[], None]):
		super().RegisterCallback(action)

	def AddIcon(self, name: str):
		super().AddIcon(name)

	def PopIcon(self):
		super().PopIcon()

	def ClearIcons(self):
		super().ClearIcons()


class VerbsHolder(eVerbsHolder):
	def __new__(cls, parent):
		return parent

	def AddVerb(self, name: str, action: Callable[[], None]):
		super().AddVerb(name, action)
