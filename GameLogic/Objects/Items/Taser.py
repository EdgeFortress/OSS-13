from Objects.Item import Item
from Objects.Projectile import Projectile
from Engine.World import CreateObject, Object

class Taser(Item):
	canBeSpawned = True
	defName = "Taser"
	defSprite = "taser"

	def InteractWith(self, object):
		if object is None:
			return False

		if not object.position:
			return False

		self.CreateProjectile(object.position - self.position)
		return True

	def CreateProjectile(self, direction):
		projectile = CreateObject("Objects.Projectile", self.tile)
		projectile.SetShotDirection(direction)
