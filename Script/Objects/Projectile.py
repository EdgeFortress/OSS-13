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
	
	def Update(self, timeElapsed):
		super().Update(timeElapsed)

		if self.startTile is None:
			self.startTile = self.tile
		else:
			if self.startTile is not self.tile:
				if self.tile.IsDense():
					self.Hit(self.tile.GetDenseObject())

	def Hit(self, hittedObject):
		if isinstance(hittedObject, Creature):
			hittedObject.Stun()
		self.Delete()
