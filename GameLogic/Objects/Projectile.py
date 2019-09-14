from Engine.Geometry import DirectionSet, Direction
from Engine.World import Object

from Objects.Creature import Creature

class Projectile(Object):
	def __init__(self):
		super().__init__()
		self.name = "Stun Orb"
		self.layer = 100
		self.sprite = "stunorb"
		self.density = True
		self.startTile = None
		self.__absoluteSpeed = 7.0

	def BumpedTo(self, object):
		super().BumpedTo(object)
		self.__hit(object)
		return True

	def SetShotDirection(self, direction):
		direction = direction.Normalize()
		self.speed = direction * self.__absoluteSpeed

	def __hit(self, hittedObject):
		if isinstance(hittedObject, Creature):
			hittedObject.Stun()
		self.Delete()
