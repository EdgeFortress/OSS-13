from Objects.Turf import Turf

class Airlock(Turf):
	def __init__(self):
		super().__init__()
		self.name = "Airlock"
		self.sprite = "airlock"
		self.density = True

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
			self.density = True
		else:
			if not self.PlayAnimation("airlock_opening", lambda: self.__animationOpeningCallback()):
				return
			self.sprite = "airlock_opened"

	def __animationOpeningCallback(self):
		self.opened = True
		self.density = False

	def __autocloseCallback(self):
		if self.opened:
			self.Activate()

