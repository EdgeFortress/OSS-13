from __future__ import annotations

from Engine_Geometry import *

from math import atan2, cos, sin
from typing import List

class Vector2D:
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
		"""Returns the angle of this vector from the x axis"""
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

	def __repr__(self):
		return '<Vector2D: {:}, {:}>'.format(self.x, self.y)

	def __str__(self):
		return '<{:}, {:}>'.format(self.x, self.y)

	@classmethod
	def _FromEngineVec(cls, engineVec):
		return cls(engineVec.x, engineVec.y)


class Vector:
	def __init__(self, x=0, y=0, z=0):
		self.x = x
		self.y = y
		self.z = z

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

	def __repr__(self):
		return '<Vector: {:}, {:}, {:}>'.format(self.x, self.y, self.z)

	def __str__(self):
		return '<{:}, {:}, {:}>'.format(self.x, self.y, self.z)

	@classmethod
	def __FromEngineVec(cls, engineVec):
		return cls(engineVec.x, engineVec.y, engineVec.z)


class Direction(eDirection):
	NONE		= eDirection.NONE
	SOUTH		= eDirection.SOUTH
	WEST		= eDirection.WEST
	NORTH		= eDirection.NORTH
	EAST		= eDirection.EAST
	SOUTH_WEST	= eDirection.SOUTH_WEST
	NORTH_EAST	= eDirection.NORTH_EAST
	NORTH_EAST	= eDirection.NORTH_EAST
	SOUTH_EAST	= eDirection.SOUTH_EAST
	CENTER		= eDirection.CENTER


def DirectionToVect(dir: Direction) -> Vector2D:
	return Vector2D._FromEngineVect(eDirectionToVect(dir))


def InvertDirection(dir: Direction) -> Direction:
	return eInvertDirection(dir)


class DirectionSet(eDirectionSet):
	def __init__(self, impl = eDirectionSet()):
		self._impl = impl

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
