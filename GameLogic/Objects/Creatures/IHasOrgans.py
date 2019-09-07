import itertools
import types

from Objects.Items.Organ import Organ
from Objects.Items.Organs.Hand import Hand

class IHasOrgans():
	def __init__(self):
		super().__init__()
		self.organs = types.SimpleNamespace()
		self.organs.__all = []
		self.organs.__hands = []
		self.organs.__handsCycle = None
		self.organs.__activeHand = None

		self.CreateOrgans()

	@property
	def activeHand(self):
		return self.organs.__activeHand

	@activeHand.setter
	def activeHand(self, hand):
		if not isinstance(hand, Hand):
			raise TypeValue("activeHand must be a type of Hand!")
		if not hand in self.organs.__hands:
			raise ValueError("activeHand must be hand which is belong to creature!")
		self.organs.__activeHand = hand
		self.organs.__handsCycle = itertools.dropwhile(lambda x: x != hand, self.organs.__handsCycle)

	@property
	def hands(self):
		return self.organs.__hands

	# Define organs with AddOrgan calls
	def CreateOrgans(self):
		raise NotImplementedError("Must be defined in derived classes!")

	# Returns added organ
	def AddOrgan(self, organ) -> Organ:
		if not isinstance(organ, Organ):
			raise TypeError("Argument organ must be of type Organ!")

		self.organs.__all.append(organ)

		if isinstance(organ, Hand):
			self.organs.__hands.append(organ)
			self.organs.__handsCycle = itertools.cycle(self.organs.__hands)
			self.SwitchHand()

		return organ

	def RemoveOrgan(self, organ):
		if not isinstance(organ, Organ):
			raise TypeError("Argument organ must be of type Organ!")

		self.organs.__all.remove(organ)

		if isinstance(organ, Hand):
			self.organs.__hands.remove(organ)
			if self.organs.__hands:
				self.organs.__handCycle = itertools.cycle(self.organs.__hands)
				self.SwitchHand()
			else:
				self.organs.__handCycle = None
				self.organs.__activeHand = None
	
	# Switch activeHand and returns it
	def SwitchHand(self) -> Hand:
		self.organs.__activeHand = next(self.organs.__handsCycle)
