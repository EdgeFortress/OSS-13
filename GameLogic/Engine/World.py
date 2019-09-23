from __future__ import annotations

from Engine_World import *
from Engine_Geometry import *

import Engine.Server
from Engine.Geometry import Vector, Vector2D, DirectionSet, NextDirection

from datetime import timedelta
from typing import Callable

class World(eWorld):
	"""
	World class

	Methods
	-------
	GetObjectById(id: int) -> Object
		Returns object with given ID, if exists.

		Note: each object in world has unique persistent id.
		But be careful: when object is destroyed, its id can be reassigned.

	GetMap() -> Map
		Returns world's map

		Note: For now world has only one map. This will be changed in next builds.

	"""
	def __init__(self, impl):
		self._impl = impl

	def GetObjectById(self, id: int) -> Object:
		"""Returns object with given ID, if exists"""
		return self._impl.GetObjectById(id)

	def GetMap(self) -> Map:
		"""Returns world's map"""
		return self._impl.GetMap()


class Map(eMap):
	"""
	Map is 3D cuboid of tiles.

	Properties
	----------
	size: Vector
		3-dimensional size of map
		Read-only

	Methods
	-------
	GetTile(pos: Vector) -> Tile
		Returns tile with given position
		None if pos is not included to [{0, 0, 0}, size)

	"""
	def __init__(self, impl):
		self._impl = impl

	@property
	def size(self) -> Vector:
		return Vector._FromEngineVec(self._impl.size())

	def GetTile(self, pos: Vector) -> Tile:
		return Tile(self._impl.GetTile(eVec3i(pos.x, pos.y, pos.z)))


class Tile(eTile):
	"""
	One tile of map

	Properties
	----------
	x, y, z: int
		Read-only

	pos: Vector
		Read-only

	map: Map
		Read-only

	Methods
	-------
	IsDense(directions: DirectionSet) -> bool
		True if one of tile's objects "solidity" blocks at least one of directions

	IsSpace() -> bool
		True if tile hasn't floor and wall

	GetDenseObject(directions: DirectionSet) -> Object
		Returns first tile's object with "solidity" which blocks one of given directions

	"""
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
		return self._impl.IsDense(directions._impl)

	def IsSpace(self) -> bool:
		return self._impl.IsSpace()

	def GetDenseObject(self, directions: DirectionSet) -> Object:
		return self._impl.GetDenseObject(directions._impl)


class VerbsHolder(eVerbsHolder):
	"""
	Inheritance from this class allow to define special actions without
	arguments which can be called with command from console

	VerbsHolder is registrated with unique name, so player can call it's verbs.

	For example, class Player registrated with name "Player".
	So player's verb "Drop" can be called with next command: "Player.Drop"

	Methods
	-------
	AddVerb(name: str, action: Callable[[Player], None])
		add new verb

		Parametres
		----------
		name: str
			verb's key. Use name to call verb from console

	"""

	def __init__(self, impl):
		self._impl = impl

	def AddVerb(self, name: str, action: Callable[[Engine.Server.Player], None]):
		wrapper = lambda player: action(Engine.Server.Player(player))
		eVerbsHolder.AddVerb(self._impl, name, wrapper)


class Object(eObject, VerbsHolder):
	"""
	Any game object

	Static properties
	-----------------

	canBeSpawned: bool
		determine whether object can be spawned via Spawn Menu. Doesn't affect whether object can be created with CreateObject
		Object with defName == None can't be spawned even if canBeSpawned set to True

	Note: default properties are static (prefix "def"). This means, that you can see them in 
		Spawn window and other places where we use not objects instances themself, but types

	defName: str
		default type's object's name
		Type can't be created if defName == None

	defSprite: str
		default type's object's sprite

	defDescription: str
		default type's object's description
		Note: doesn't display anywhere for now


	Properties
	----------
	name: str
		visible object's name; used only for displaying

	sprite: str
		object's sprite's ID

	layer: int
		drawing layer belongs [0, 100]. Greater value, later drawn

	density: bool
		when False, object ignores other objects solidity

	solidity: DirectionSet
		solid directions of object

		- if one of 4 main "directed" directions is set (NORTH, SOUTH, WEST, EAST),
			then no one dense object can walk in to tile from this direction (or out from this tile)
		- if CENTER is set,
			then no one dense object can walk in this tile from any direction
		- any composite direction is breaks down to main directions

	invisibility: int
		invisibility flags of object (each bit is different type of invisibility)

	tile: Tile
		tile where object is placed. Can be None, if object is placed nowhere.

	position: Vector
		coordinates of object's tile. Vector2D(-1, -1) if tile is None.

	speed: Vector2D
		physical speed of object

	moveSpeed: float
		moving speed. If object can move, it will move with this speed

	isWall: bool
		True if object is wall. Used for atmosphere subsystem

		Note: don't use it! Will be remove in next builds.

	isFloor: bool
		True if object is floor. Used for atmosphere subsystem

		Note: don't use it! Will be remove in next builds.


	Methods
	-------
	Update(timeElapsed: timedelta)
		is called by Engine each tick

		Note: don't call it from scripts!
			should be overwritten if specific Update logic is needed

		Parametres
		----------
		timeElapsed: timedelta
			time was consumed by last tick

	InteractedBy(obj: Object) -> bool
		is called by Engine on interactions.
		Means that current Object is tried to be interacted by someone with obj Object

		Note: don't call it from scripts!
			should be overwritten if specific OnInteraction logic is needed

		Returns
		-------
		Should return True, if interaction is successful. False if interaction is impossible.

	BumpedTo(obj: Object) -> bool
		is called by Engine when object is bumps to obj Object
		Object can be bumped to something only when moving with physical speed.
		When moving, additional checks don't allow bumping.

		Note: don't call it from scripts!
			should be overwritten if specific OnBump logic is needed

		Returns
		-------
		Should return True, if processed. False if bump is impossible.

	IsCloseTo(obj: Object) -> bool
		returns True if distance between object and obj < 2 (same tile, or neighbour, even diagonally)

	Move(dir: Vector2D):
		set order to move somewhere with moving speed

		Note: don't invoke bumps. If moving is imposible due obstacle, order will be ignored

		Parametres
		----------
		dir: Vector2D
			direction to move. Each coordinate should belong {-1, 0, 1}

	MoveZ(self, dir: int):
		set order to move by Z-level

		Parametres
		----------
		dir: int
			z-level direction to move, should be 1 or -1

	AddComponent(self, type: str):
		create and add "type" component to object

		Note: for now obly "Control" component is available

	GetComponent(self, type: str) -> Component:
		get component by it's type
		None if object doesn't contain such component

	AddObject(self, obj: Object):
		add obj to object's content

		Note: obj.tile will be set to object's tile

	RemoveObject(self, obj: Object) -> bool:
		remove obj from object's content
		Returns False if object doesn't contain obj

	SetSpriteState(state: ItemSpriteState):
		set object's sprite state

	PlayAnimation(id: str, callback: Callable[[], None] = None) -> bool:
		play animation with given id and call callback when finished

		Note: "animation" is animated sprite. Thus "id" is sprite id.

	Delete(self):
		delete object

	_updateIcons(self):
		is called by Engine when icons update is needed

		Note: don't call it from scripts!
			you can overwrite it to add some icons with _pushToIcons method

	 _pushToIcons(self, icon: eIcon):
		add icon to be drawn as object part/overlay

		Note: use it only in _updateIcons definition

		Parametres
		----------
		icon: eIcon
			engine type of Icon. Can be received from ResourceManager

	"""

	canBeSpawned = True
	defName = None
	defSprite = None
	defDescription = None

	defRotatable = False

	def __init__(self):
		eObject.__init__(self)
		VerbsHolder.__init__(self, self)
		self.name = str(self.defName)
		self.sprite = str(self.defSprite)
		self.description = str(self.defDescription)

		self.__rotatable = self.defRotatable
		
		self._registrateVerbs()

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
	def rotatable(self) -> bool:
		return self.__rotatable
	@rotatable.setter
	def rotatable(self, value: bool):
		self.__rotatable = value

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
		raise TypeError("Bad component type!")

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

	def _registrateVerbs(self):
		self.AddVerb("Delete", lambda player: self.__deleteVerb())
		if self.rotatable:
			self.AddVerb("Rotate", lambda player: self.__rotateVerb())

	def __deleteVerb(self):
		self.Delete()

	def __rotateVerb(self):
		print(self.name + " rotate")


def CreateObject(typeKey: str, tile: Tile = None) -> Object:
	"""Create object by it's type

	Parametres
	----------
	typeKey: str
		object's type key, which consists of module path and object name.

		Example: in order to create grey uniform with type's name "Grey", defined in module "Objects.Items.Clothes.Uniform",
			typeKey should be "Objects.Items.Clothes.Uniform.Grey"

	tile: Tile
		tile, where object should be created. Can be None, if you don't need to place it yet.

	Returns
	-------
	Object
		spawned object

	Example
	-------
		ghost = CreateObject("Objects.Creatures.Ghost")

	"""

	if isinstance(tile, Tile):
		# called from scripts
		return eCreateObject(typeKey, tile._impl)
	else:
		# called from engine
		return eCreateObject(typeKey, tile)


class Component(eComponent):
	"""
	object's logic component

	Methods
	-------
	Update(timeElapsed: timedelta)
		is called by Engine each tick

		Note: don't call it from scripts!
			should be overwritten if specific Update logic is needed

		Parametres
		----------
		timeElapsed: timedelta
			time was consumed by last tick

	GetOwner() -> Object
		returns component's owner object.
		Couldn't be None

	"""

	def __init__(self, impl):
		self._impl = impl

	def Update(self, timeElapsed: timedelta):
		self._impl.Update(timedelta)

	def GetOwner(self) -> Object:
		return self._impl.GetOwner()


class Control(eControl, Component):
	"""
	Object controlling logic component

	Used to provide control interface for Player and AI

	Attributes
	---------
	ui: ControlUI
		UI used for controlling object
		Read-only

	seeInvisibleAbility: int
		invisibility flags which are visible for Player (each bit makes visible specific invisibility type)

	Methods
	-------
	GetAndDropMoveOrder() -> Vector2D
		returns last move order received
		Internal order buffer will be dropped

	GetAndDropMoveZOrder -> int
		returns last Z-move order received
		Internal order buffer will be dropped

	GetAndDropClickedObject() -> Object
		returns last clicked object
		Internal order buffer will be dropped

	"""

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
	"""
	UI used to object controlling

	You can see this UI drawned over TileGrid usually.
	It consists of elements which are drawn with common sprites.
	For each element callback can be registered to be called when element is clicked.

	Note: ControlUI resolution is virtual and independent of real player screen sizes.
		UI will be automatically scaled on client side.

	Attributes
	----------
	resolution: Vector2D
		drawable field resolution (Vector2D(15 * 32, 15 * 32) for now)
		Read-only

	center: Vector2D
		drawable field center
		Read-only

	iconSize: Vector2D
		one element size (Vector2D(32, 32) for now)
		Read-only

		Note: values are given only as an example, don't rely on them

	Methods
	-------

	UpdateElement(element: ControlUIElement)
		update element

		You can add created element with this method, or update previous with same id

	RemoveElement(elementId: str)
		remove element by it's id

	"""

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
	"""
	One ControlUI's element. Will be drawn on client and can be clicked.

	Attributes
	----------
	id: str
		element's id. Elements with same id are merged.

	position: Vector2D
		on-screen position. Count it relatively ControlUI.resolution

	Methods
	-------

	RegistrateCallback(action: Callable[[], None])
		registrate action to be called on click

	AddIcon(name: str)
		add icon to be drawn on top of previous icons

	PopIcon()
		remove last added icon

	ClearIcons()
		remove all icons

	"""

	def __init__(self):
		super().__init__()

	@property
	def id(self) -> str:
		return super().id
	@id.setter
	def id(self, value: str):
		super(ControlUIElement, self.__class__).id.fset(self, value)

	@property
	def position(self) -> Vector2D:
		return Vector2D._FromEngineVec(super().position)
	@position.setter
	def position(self, value: Vector2D):
		super(ControlUIElement, self.__class__).position.fset(self, eVec2i(value.x, value.y))

	def RegisterCallback(self, action: Callable[[], None]):
		super().RegisterCallback(action)

	def AddIcon(self, name: str):
		super().AddIcon(name)

	def PopIcon(self):
		super().PopIcon()

	def ClearIcons(self):
		super().ClearIcons()
