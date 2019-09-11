from Engine_Server import Server, ResourceManager, ItemSpriteState
from Object import Object
from Objects.Item import Item
from Objects.Items.Clothing import Clothing, MobSlot
from Objects.Creatures.IHasOrgans import IHasOrgans

class Creature(Object, IHasOrgans):
	def __init__(self):
		Object.__init__(self)
		IHasOrgans.__init__(self)

		self.layer = 75
		self.name = "Creature"

		self.__seeInvisibleAbility = False

		self.__clothes = {}

		self.AddComponent("Control")
		self.control = self.GetComponent("Control")
		self.DefineUI(self.control.ui)

		self.AddVerb("drop", lambda player: self.Drop())

	@property
	def seeInvisibleAbility(self):
		return self.__seeInvisibleAbility

	@seeInvisibleAbility.setter
	def seeInvisibleAbility(self, value):
		if not isinstance(value, int):
			raise TypeError("seeInvisibleAbility must be int!")
		if self.control is not None:
			self.__seeInvisibleAbility = value
			self.control.seeInvisibleAbility = value

	def DefineUI(self, ui):
		pass

	def Update(self, timeElapsed):
		moveOrder = self.control.GetAndDropMoveOrder()
		if moveOrder: self.OnMoveOrder(moveOrder)

		moveZOrder = self.control.GetAndDropMoveZOrder()
		if moveZOrder: self.OnMoveZOrder(moveZOrder)

		clickedObject = self.control.GetAndDropClickedObject()
		if clickedObject: self.OnObjectClick(clickedObject)

	def OnMoveOrder(self, order):
		self.Move(order)

	def OnMoveZOrder(self, order):
		self.MoveZ(order)

	def OnObjectClick(self, object):
		self.TryInteractWith(object)

	def InteractedBy(self, object) -> bool:
		print("InteractedBy " + object.name)
		if isinstance(object, Creature):
			for _, clothing in self.__clothes.items():
				if object.Take(clothing):
					return True
			return False

		if isinstance(object, Clothing):
			return self.PutOn(object)

		return False

	def TryInteractWith(self, object) -> bool:
		if not isinstance(object, Object):
			return False

		if self.activeHand:
			if not self.activeHand.isEmpty:
				if object.InteractedBy(self.activeHand.holdedItem):
					return True

				if self.activeHand.holdedItem.InteractWith(object):
					return True

				return True

		if isinstance(object, Item):
			if self.Take(object):
				return True

		object.InteractedBy(self)
		return False

	def Take(self, object) -> bool:
		if not self.activeHand:
			return False
		if not self.activeHand.isEmpty:
			return False
		if not self.IsCloseTo(object):
			return False

		self.AddObject(object)
		self.activeHand.holdedItem = object
		return True

	def PutOn(self, clothing) -> bool:
		if not isinstance(clothing, Clothing):
			return False

		if clothing.slot in [MobSlot.NONE, MobSlot.LHAND, MobSlot.RHAND]:
			return False

		if clothing.slot in self.__clothes:
			return False

		self.AddObject(clothing)
		self.__clothes[clothing.slot] = clothing
		clothing.SetSpriteState(ItemSpriteState.ON_MOB)
		return True

	def GetSlotItem(self, mobSlot) -> Item:
		if not isinstance(mobSlot, MobSlot):
			return None

		return self.__clothes.get(mobSlot)

	def RemoveObject(self, object) -> bool:
		for hand in self.hands:
			if object is hand.holdedItem:
				hand.holdedItem = None

		if isinstance(object, Clothing):
			clothingOnMob = self.__clothes.get(object.slot)
			if clothingOnMob and clothingOnMob is object:
				self.__clothes.pop(object.slot)

		return super().RemoveObject(object)

	def Drop(self):
		if not self.activeHand.isEmpty:
			self.activeHand.holdedItem.tile = self.tile
			self.activeHand.holdedItem = None

	def Stun(self):
		print(self.name + " stunned!")

	def _pushStateToIcons(self, slot):
		item = self.GetSlotItem(slot)
		if item is None:
			return

		state = ItemSpriteState.ON_MOB

		if slot == MobSlot.LHAND:
			state = ItemSpriteState.IN_HAND_LEFT
		elif slot == MobSlot.RHAND:
			state = ItemSpriteState.IN_HAND_RIGHT

		rm = Server.RM
		icon = rm.GetIcon(item.sprite, state)
		if icon:
			super()._pushToIcons(icon)

	def _updateIcons(self):
		super()._updateIcons()

		self._pushStateToIcons(MobSlot.UNIFORM)
		self._pushStateToIcons(MobSlot.LHAND)
		self._pushStateToIcons(MobSlot.RHAND)
