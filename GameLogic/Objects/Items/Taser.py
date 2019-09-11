from Objects.Item import Item
from Objects.Projectile import Projectile
from Engine_World import CreateObject

class Taser(Item):
	def __init__(self):
		super().__init__()
		self.name = "Python Taser"
		self.sprite = "taser"

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
