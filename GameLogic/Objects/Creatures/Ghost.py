from Objects.Creature import Creature
from Engine.World import CreateObject, Object
from Engine.Geometry import Vector2D

class Ghost(Creature):
	defName = "Ghost"
	defSprite = "ghost"

	def __init__(self):
		Creature.__init__(self)
		self.layer = 80
		self.density = False
		self.seeInvisibleAbility = 1
		self.invisibility = 1

	@property
	def hostControl(self):
		return self._hostControl

	@hostControl.setter
	def hostControl(self, value):
		self._hostControl = value

	# IHasOrgans methods
	def CreateOrgans(self):
		pass


def Ghostize(player):
	mob = player.control.GetOwner()

	if not isinstance(mob, Ghost):
		tile = player.control.GetOwner().tile
		ghost = CreateObject("Objects.Creatures.Ghost.Ghost", tile)
		ghost.hostControl = player.control
		player.control = ghost.GetComponent("Control")
	else:
		ghost = mob
		player.control = ghost.hostControl
		ghost.Delete()
