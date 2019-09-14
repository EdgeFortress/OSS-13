from __future__ import annotations

from Engine_World import *
from Engine_Geometry import *

from Engine.Geometry import Vector, Vector2D, DirectionSet

from datetime import timedelta
from typing import Callable

class World(eWorld):
	def __init__(self, impl):
		self._impl = impl

	def GetObjectById(self, id: int) -> Object:
		return self._impl.GetObjectById(id)

	def GetMap(self) -> Map:
		return self._impl.GetMap()


class Map(eMap):
	def __init__(self, impl):
		self._impl = impl

	@property
	def size(self) -> Vector:
		return Vector._FromEngineVec(self._impl.size())

	def GetTile(self, pos: Vector) -> Tile:
		return Tile(self._impl.GetTile(eVec3i(pos.x, pos.y, pos.z)))


class Tile(eTile):
	def __init__(self, impl):
		self._impl = impl

	@property
	def x(self) -> int:
		return self._impl.x
	@property
	def y(self) -> int:
		return self._impl.y
	@property
	def z(self) -> int:
		return self._impl.z

	@property
	def pos(self) -> Vector:
		return Vector._FromEngineVec(self._impl.pos)

	@property
	def map(self) -> Map:
		return Map(self._impl.map)

	def IsDense(self, directions: DirectionSet) -> bool:
		return self._impl.IsDense(directions)

	def IsSpace(self) -> bool:
		return self._impl.IsSpace()

	def GetDenseObject(self, directions: DirectionSet) -> Object:
		return self._impl.GetDenseObject(directions)


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
		return DirectionSet(super().solidity)
	@solidity.setter
	def solidity(self, value: DirectionSet):
		super(Object, self.__class__).solidity.fset(self, value._impl)

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
		super(Object, self.__class__).tile.fset(self, value._impl)

	@property
	def position(self) -> Vector2D:
		return Vector2D._FromEngineVec(super().position)
	@position.setter
	def position(self, value: Vector2D):
		super(Object, self.__class__).position.fset(self, eVec2i(value.x, value.y))

	@property
	def speed(self) -> Vector2D:
		return Vector2D._FromEngineVec(super().speed)
	@speed.setter
	def speed(self, value: Vector2D):
		super(Object, self.__class__).speed.fset(self, eVec2f_32(value.x, value.y))

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

	def Move(self, dir: Vector2D):
		super().Move(eVec2i(dir.x, dir.y))

	def MoveZ(self, dir: int):
		super().MoveZ(dir)

	def AddComponent(self, type: str):
		super().AddComponent(type)

	def GetComponent(self, type: str) -> Component:
		cmp = super().GetComponent(type)
		if isinstance(cmp, eControl):
			return Control(cmp)
		return Component(cmp)

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
	if isinstance(tile, Tile):
		# called from scripts
		return eCreateObject(type, tile._impl)
	else:
		# called from engine
		return eCreateObject(type, tile)


class Component(eComponent):
	def __init__(self, impl):
		self._impl = impl

	def Update(self, timeElapsed: timedelta):
		self._impl.Update(timedelta)

	def GetOwner() -> Object:
		return self._impl.GetOwner()


class Control(eControl, Component):
	def __init__(self, impl):
		self._impl = impl

	@property
	def ui(self) -> ControlUI:
		return ControlUI(self._impl.ui)

	@property
	def seeInvisibleAbility(self) -> int:
		return self._impl.seeInvisibleAbility
	@seeInvisibleAbility.setter
	def seeInvisibleAbility(self, value: int):
		self._impl.seeInvisibleAbility = value

	def GetAndDropMoveOrder(self) -> Vector2D:
		return Vector2D._FromEngineVec(self._impl.GetAndDropMoveOrder())

	def GetAndDropMoveZOrder(self) -> int:
		return self._impl.GetAndDropMoveZOrder()

	def GetAndDropClickedObject(self) -> Object:
		return self._impl.GetAndDropClickedObject()


class ControlUI(eControlUI):
	def __init__(self, impl):
		self._impl = impl

	@property
	def resolution(self) -> Vector2D:
		return Vector2D._FromEngineVec(self._impl.resolution)

	@property
	def center(self) -> Vector2D:
		return Vector2D._FromEngineVec(self._impl.center)

	@property
	def iconSize(self) -> Vector2D:
		return Vector2D._FromEngineVec(self._impl.iconSize)


	def UpdateElement(self, element: ControlUIElement):
		self._impl.UpdateElement(element)

	def RemoveElement(self, elementId: str):
		self._impl.RemoveElement(elementId)


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
	def position(self) -> Vector:
		return Vector2D._FromEngineVec(super().position)
	@position.setter
	def position(self, value: Vector):
		super(ControlUIElement, self.__class__).position.fset(self, eVec2i(value.x, value.y))

	def RegisterCallback(self, action: Callable[[], None]):
		super().RegisterCallback(action)

	def AddIcon(self, name: str):
		super().AddIcon(name)

	def PopIcon(self):
		super().PopIcon()

	def ClearIcons(self):
		super().ClearIcons()
