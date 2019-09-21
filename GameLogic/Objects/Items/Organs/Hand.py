from Objects.Item import Item
from Objects.Items.Organ import Organ

class Hand(Organ):
	def __init__(self):
		super().__init__()
		self.__item = None

	@property
	def holdedItem(self):
		return self.__item

	@holdedItem.setter
	def holdedItem(self, value):
		if not isinstance(value, Item) and value is not None:
			raise TypeError("holdedItem must be Item!")
		self.__item = value

	@property
	def isEmpty(self):
		return self.__item is None
