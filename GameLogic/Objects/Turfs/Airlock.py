from Engine.Geometry import Direction, DirectionSet
from Objects.Turf import Turf

class Airlock(Turf):
	defName = "Airlock"
	defSprite = "airlock"

	def __init__(self):
		super().__init__()

		self.__closedSolidity = DirectionSet()
		self.__closedSolidity.Add([Direction.CENTER])
		
		self.solidity = self.__closedSolidity

		self.opened = False
		self.locked = False

	def InteractedBy(self, object):
		if not self.IsCloseTo(object):
			return False
		self.Activate()
		return True

	def Activate(self):
		if self.locked:
			if not self.opened:
				self.PlayAnimation("airlock_closed_animation")
			return

		if self.opened:
			if not self.PlayAnimation("airlock_closing"):
				return
			self.sprite = "airlock"
			self.opened = False
			self.solidity = self.__closedSolidity
		else:
			if not self.PlayAnimation("airlock_opening", lambda: self.__animationOpeningCallback()):
				return
			self.sprite = "airlock_opened"

	def Lock(self):
		self.locked = True

	def Unlock(self):
		self.locked = False

	def __animationOpeningCallback(self):
		self.opened = True
		self.solidity = DirectionSet()

	def __autocloseCallback(self):
		if self.opened:
			self.Activate()

