from Object import Object
from Objects.Creature import Creature

class Projectile(Object):
	def __init__(self):
		super().__init__()
		self.name = "Stun Orb"
		self.layer = 100
		self.sprite = "stunorb"
		self.density = False
		self.startTile = None
		self.__absoluteSpeed = 7.0
		self.__direction = None
	
	def Update(self, timeElapsed):
		super().Update(timeElapsed)

		if self.startTile is None:
			self.startTile = self.tile
		else:
			if self.startTile is not self.tile:
				if self.tile.IsDense():
					self.Hit(self.tile.GetDenseObject())

	def SetShotDirection(self, direction):
		direction = direction.Normalize()
		self.speed = direction * self.__absoluteSpeed
		self.__direction = direction

	def Hit(self, hittedObject):
		if isinstance(hittedObject, Creature):
			hittedObject.Stun()
		self.Delete()
