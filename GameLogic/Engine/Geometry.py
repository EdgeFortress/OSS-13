from __future__ import annotations

from Engine_Geometry import *

from math import atan2, cos, sin
from typing import List, Tuple

class Vector2D:
	"""
	Vector2D Class

	Attributes
	----------
	x, y: float
		vector coordinates

	Methods
	-------
	Magnitude() -> float
		count vector's length

	Normalize() -> Vector2D
		get unit vector with same direction

	Direction() -> Direction
		get Direction where vector is directed (1 direction of 8 options)

	Angle() -> float
		get vector's angle from the x-axis

	Rotate(angleToRotate: float) -> Vector2D
		create vector rotated by given angle relatively initial

	"""
	def __init__(self, x, y):
		self.x = x
		self.y = y

	def Magnitude(self) -> float:
		"""Returns the magnitude of this vector"""
		return (self.x**2 + self.y**2)**0.5

	def Normalize(self) -> Vector2D:
		"""Returns the unit vector of this vector"""
		mag = self.Magnitude()
		if mag == 0:
			return Vector2D(self.x, self.y)
		else:
			x = self.x/mag
			y = self.y/mag
			return Vector2D(x, y)

	def Direction(self) -> Direction:
		return eVec2f(self.x, self.y).GetDirection()

	def Angle(self) -> float:
		"""Returns the angle of this vector from the x-axis"""
		return atan2(self.y, self.x)

	def Rotate(self, angleToRotate: float) -> Vector2D:
		"""Rotates this vector by an input angle in radians"""
		mag = self.Magnitude()
		ang = self.Angle()
		x = mag * cos(ang + angleToRotate)
		y = mag * sin(ang + angleToRotate)
		return Vector2D(x, y)

	def __add__(self, obj):
		if isinstance(obj, Vector2D):
			return Vector2D(self.x + obj.x, self.y + obj.y)

	def __radd__(self, obj):
		if obj == 0 or obj is None:
			return self
		else:
			return self.__add__(obj)

	def __sub__(self, obj):
		if isinstance(obj, Vector2D):
			return Vector2D(self.x - obj.x, self.y - obj.y)

	def __pos__(self):
		return Vector2D(self.x, self.y)

	def __neg__(self):
		return Vector2D(-self.x, -self.y)

	def __mul__(self, obj):
		if isinstance(obj, Vector2D):
			return self.x * obj.x + self.y * obj.y
		else:
			return Vector2D(self.x * obj, self.y * obj)

	def __rmul__(self, obj):
		return self.__mul__(obj)

	def __truediv__(self, obj):
		x = self.x / obj
		y = self.y / obj
		return Vector2D(x, y)

	def __floordiv__(self, obj):
		x = self.x // obj
		y = self.y // obj
		return Vector2D(x, y)

	def __eq__(self, other):
		return self.x == other.x and self.y == other.y

	def __ne__(self, other):
		return not self == other

	def __repr__(self):
		return '<Vector2D: {:}, {:}>'.format(self.x, self.y)

	def __str__(self):
		return '<{:}, {:}>'.format(self.x, self.y)

	@classmethod
	def _FromEngineVec(cls, engineVec):
		return cls(engineVec.x, engineVec.y)


class Vector:
	"""
	Vector Class

	Attributes
	----------
	x, y, z: float
		vector coordinates

	Methods
	-------
	Magnitude() -> float
		count vector's length

	Normalize() -> Vector
		get unit vector with same direction

	"""
	def __init__(self, x=0, y=0, z=0):
		self.x = x
		self.y = y
		self.z = z
	
	def xy(self) -> Vector2D:
		return Vector2D(self.x, self.y)
	
	def Magnitude(self) -> float:
		"""Returns the magnitude of this vector"""
		return (self.x**2 + self.y**2 + self.z**2)**0.5

	def Normalize(self) -> Vector:
		"""Returns the unit vector of this vector"""
		mag = self.Magnitude()
		if mag == 0:
			return Vector(self.x, self.y, self.z)
		else:
			x = self.x / mag
			y = self.y / mag
			z = self.z / mag
			return Vector(x, y, z)

	def __add__(self, obj):
		if isinstance(obj, Vector):
			return Vector(self.x + obj.x, self.y + obj.y, self.z + obj.z)

	def __radd__(self, obj):
		if obj == 0 or obj is None:
			return self
		else:
			return self.__add__(obj)

	def __sub__(self, obj):
		if isinstance(obj, Vector):
			return Vector(self.x - obj.x, self.y - obj.y, self.z - obj.z)

	def __pos__(self):
		return Vector(self.x, self.y, self.z)

	def __neg__(self):
		return Vector(-self.x, -self.y, -self.z)

	def __mul__(self, obj):
		if isinstance(obj, Vector):
			return self.x * obj.x + self.y * obj.y + self.z * obj.z
		else:
			return Vector(self.x * obj, self.y * obj, self.z * obj)

	def __rmul__(self, obj):
		return self.__mul__(obj)

	def __truediv__(self, obj):
		x = self.x / obj
		y = self.y / obj
		z = self.z / obj
		return Vector(x, y, z)

	def __floordiv__(self, obj):
		x = self.x // obj
		y = self.y // obj
		z = self.z // obj
		return Vector(x, y, z)

	def __eq__(self, other):
		return self.x == other.x and self.y == other.y and self.z == other.z

	def __ne__(self, other):
		return not self == other

	def __repr__(self):
		return '<Vector: {:}, {:}, {:}>'.format(self.x, self.y, self.z)

	def __str__(self):
		return '<{:}, {:}, {:}>'.format(self.x, self.y, self.z)

	@classmethod
	def _FromEngineVec(cls, engineVec):
		return cls(engineVec.x, engineVec.y, engineVec.z)

class Direction(eDirection):
	"""
	Direction enumeration
	8 existing world directions, Center and None
	"""
	NONE        = eDirection.NONE
	SOUTH       = eDirection.SOUTH
	WEST        = eDirection.WEST
	NORTH       = eDirection.NORTH
	EAST        = eDirection.EAST
	SOUTH_WEST  = eDirection.SOUTH_WEST
	NORTH_WEST  = eDirection.NORTH_WEST
	NORTH_EAST  = eDirection.NORTH_EAST
	SOUTH_EAST  = eDirection.SOUTH_EAST
	TOP         = eDirection.TOP
	BOTTOM      = eDirection.BOTTOM
	CENTER      = eDirection.CENTER


def DirectionToVect(dir: Direction) -> Vector2D:
	"""Cast Direction to Vector2D

	Attributes
	----------
	dir: Direction
		Direction to be casted to Vector2D

	Returns
	-------
	Vector2D
		resulted vector
		all coordinates equal one of {-1, 0, 1}

	Example:
	-------
	DirectionToVect(Direction.NORTH_EAST) -> Vector2D(1, 1)

	"""
	return Vector2D._FromEngineVect(eDirectionToVect(dir))


def InvertDirection(dir: Direction) -> Direction:
	"""Invert Direction

	Parameters
	----------
	dir: Direction
		Direction to be inverted

	Returns
	-------
	Direction
		opposite direction (mirrored)

	Example:
	-------
	InvertDirection(Direction.NORTH_EAST) -> Directon.SOUTH_WEST

	"""
	return eInvertDirection(dir)


def NextDirection(dir: Direction) -> Direction:
	"""Get next Direction clock-wise. Works only with plain directions."""

	if dir == Direction.NONE:
		return Direction.WEST
	elif dir == Direction.SOUTH:
		return Direction.WEST
	elif dir == Direction.WEST:
		return Direction.NORTH
	elif dir == Direction.NORTH:
		return Direction.EAST
	elif dir == Direction.EAST:
		return Direction.SOUTH
	raise TypeError("Bad direction type!")


class DirectionSet():
	"""
	A class used to represent a set of directions. You can add directions and check which are already added.
	Any composite direction will be broken to its components.

	Methods
	-------
	Add(List[Direction])
		Add directions from list to set. With composite direction both components will be added.

	Remove(List[Direction])
		Remove list of directions from set. For composite direction, it's components will be removed.

	DoesExistOne(List[Direction]) -> bool
		Check if one of directions is exist in set.

	DoExistAll(List[Direction]) -> bool
		Check if all of directions are exist in set.

	Reset()
		Remove all directions from set.

	"""

	def __init__(self, *args):
		if len(args) > 0 and isinstance(args[0], eDirectionSet):
			self._impl = args[0]
		else:
			self._impl = eDirectionSet()

		if len(args) > 0 and isinstance(args[0], list):
			self.Add(args[0])

	def Add(self, directions: List[Direction]):
		self._impl.Add(directions)

	def Remove(self, directions: List[Direction]):
		self._impl.Remove(directions)

	def DoesExistOne(self, directions: List[Direction]) -> bool:
		return self._impl.DoesExistOne(directions)

	def DoExistAll(self, directions: List[Direction]) -> bool:
		return self._impl.DoExistAll(directions)

	def Reset(self):
		self._impl.Reset()


class DirectionSetFractional():
	"""
	A class used to represent a set of directions with fractions. You can set specific fraction from 0 to 1 for each direction and check them.
	You shouldn't use composite directions with this class.
	
	Default value for each direction is 1.

	Methods
	-------
	Set(List[Tuple[Direction, float]])
		Set fractions for specific directions.

	Remove(List[Direction])
		Set fractions of specified directions to default value.

	GetFraction(Direction) -> float:
		Get fraction for specific direction.

	GetCumulativeFraction(List[Direction]) -> float:
		Get cumulative fractions for few directions

		Example:
			result = 1 * GetFraction(direction1) * GetFraction(direction2) * ...

	Reset()
		Reset all directions fractions to default value.

	"""

	def __init__(self, *args):
		if len(args) > 0 and isinstance(args[0], eDirectionSetFractional):
			self._impl = args[0]
		else:
			self._impl = eDirectionSetFractional()

		if len(args) > 0 and isinstance(args[0], list):
			self.Set(args[0])

	def Set(self, directions: List[Tuple[Direction, float]]):
		self._impl.Set(directions)

	def Remove(self, directions: List[Direction]):
		self._impl.Remove(directions)

	def GetFraction(self, direction: Direction) -> float:
		return self._impl.GetFraction(direction)

	def GetCumulativeFraction(self, directions: List[Direction]) -> float:
		return self._impl.GetCumulativeFraction(directions)

	def Reset(self):
		self._impl.Reset()
