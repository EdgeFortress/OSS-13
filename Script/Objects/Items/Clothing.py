from enum import IntEnum, auto
from Objects.Item import Item

class MobSlot(IntEnum):
	NONE = 0
	HEAD = auto()
	UNIFORM = auto()
	SHOES = auto()
	LHAND = auto()
	RHAND = auto()

class Clothing(Item):
	def __init__(self):
		super().__init__()
		self.__slot = MobSlot.NONE

	@property
	def slot(self):
		return self.__slot

	@slot.setter
	def slot(self, value):
		self.__slot = value
