from Engine.Geometry import DirectionSet, Direction
from Engine.World import Object

from Objects.Creature import Creature

class Projectile(Object):
	defName = "Stun Orb"
	defSprite = "stunorb"
	defDesc = "Taser's projectile. Watch out!"

	def __init__(self):
		super().__init__()
		self.layer = 100
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
