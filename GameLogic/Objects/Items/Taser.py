from Objects.Item import Item
from Objects.Projectile import Projectile
from Engine.World import CreateObject, Object

class Taser(Item):
	defName = "Taser"
	defSprite = "taser"

	def InteractWith(self, object):
		if object is None:
			return False

		if not object.position or object.position == self.position:
			return False

		self.CreateProjectile(object.position - self.position)
		return True

	def ClickTile(self, tile):
		if tile is None:
			return False

		if not tile.pos:
			return False
		
		self.CreateProjectile(tile.pos.xy() - self.position)
		return True

	def CreateProjectile(self, direction):
		projectile = CreateObject("Objects.Projectile.Projectile", self.tile)
		projectile.SetShotDirection(direction)
