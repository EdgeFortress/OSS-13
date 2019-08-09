from Objects.Item import Item
from Engine import CreateObject

class Taser(Item):
	speed = float(4)

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
		projectile.SetConstSpeed(direction.Normalize() * self.speed)
