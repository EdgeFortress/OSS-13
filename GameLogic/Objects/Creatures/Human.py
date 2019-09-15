import types

from Engine.World import CreateObject, ControlUIElement
from Engine.Geometry import Vector2D
from Objects.Creature import Creature
from Objects.Item import Item
from Objects.Items.Clothing import MobSlot
from Objects.Items.Organs.Hand import Hand

class Human(Creature):
	defName = "Human"
	defSprite = "human"

	def __init__(self):
		super().__init__()
		self.PutOn(CreateObject("Objects.Items.Clothes.Uniform", None))
		self.__updateActiveHandIcon()
		self.AddVerb("drop", lambda player: self.Drop())

	# IHasOrgans methods
	def CreateOrgans(self):
		self.organs.leftHand = self.AddOrgan(Hand())
		self.organs.rightHand = self.AddOrgan(Hand())

	# Creature methods
	def _defineUI(self, ui):
		self._uiFields = types.SimpleNamespace()

		element = ControlUIElement()
		element.id = "lhand"
		element.position = Vector2D(ui.center.x, ui.resolution.y - ui.iconSize.y)
		element.AddIcon("lhand_inactive")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self._uiFields.lhand = element

		element = ControlUIElement()
		element.id = "rhand"
		element.position = Vector2D(ui.center.x - ui.iconSize.x, ui.resolution.y - ui.iconSize.y)
		element.AddIcon("rhand_inactive")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self._uiFields.rhand = element

		element = ControlUIElement()
		element.id = "equip"
		element.position = Vector2D(ui.center.x - ui.iconSize.x, ui.resolution.y - ui.iconSize.y * 2)
		element.AddIcon("equip")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self._uiFields.equip = element

		element = ControlUIElement()
		element.id = "swap1"
		element.position = Vector2D(ui.center.x - ui.iconSize.x, ui.resolution.y - ui.iconSize.y * 2)
		element.AddIcon("swap1")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self._uiFields.swap1 = element

		element = ControlUIElement()
		element.id = "swap2"
		element.position = Vector2D(ui.center.x, ui.resolution.y - ui.iconSize.y * 2)
		element.AddIcon("swap2")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self._uiFields.swap2 = element

		element = ControlUIElement()
		element.id = "pocket1"
		element.position = Vector2D(ui.center.x + ui.iconSize.x, ui.resolution.y - ui.iconSize.y)
		element.AddIcon("pocket")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self._uiFields.pocket1 = element

		element = ControlUIElement()
		element.id = "pocket2"
		element.position = Vector2D(ui.center.x + ui.iconSize.x * 2, ui.resolution.y - ui.iconSize.y)
		element.AddIcon("pocket")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self._uiFields.pocket2 = element
	
	def RemoveObject(self, object) -> bool:
		for hand in self.hands:
			if object is hand.holdedItem:
				if hand is self.organs.leftHand:
					self._uiFields.lhand.PopIcon()
				else:
					self._uiFields.rhand.PopIcon()

		return super().RemoveObject(object)

	def Take(self, object) -> bool:
		if not super().Take(object):
			return False

		if self.activeHand is self.organs.leftHand:
			self._uiFields.lhand.AddIcon(object.sprite)
		else:
			self._uiFields.rhand.AddIcon(object.sprite)

		return True

	def GetSlotItem(self, mobSlot) -> Item:
		if not isinstance(mobSlot, MobSlot):
			raise TypeError("Argument organ must be of type MobSlot!")

		if mobSlot == MobSlot.LHAND:
			return self.organs.leftHand.holdedItem
		elif mobSlot == MobSlot.RHAND:
			return self.organs.rightHand.holdedItem

		return super().GetSlotItem(mobSlot)

	# Human methods
	def __uiFieldClicked(self, field):
		if field is self._uiFields.lhand:
			self.activeHand = self.organs.leftHand
			self.__updateActiveHandIcon()
		elif field is self._uiFields.rhand:
			self.activeHand = self.organs.rightHand
			self.__updateActiveHandIcon()

	def __updateActiveHandIcon(self):
		self._uiFields.lhand.ClearIcons()
		self._uiFields.rhand.ClearIcons()

		if self.activeHand is self.organs.leftHand:
			self._uiFields.lhand.AddIcon("lhand_active")
			self._uiFields.rhand.AddIcon("rhand_inactive")
		else:
			self._uiFields.lhand.AddIcon("lhand_inactive")
			self._uiFields.rhand.AddIcon("rhand_active")

		if self.organs.leftHand.holdedItem:
			self._uiFields.lhand.AddIcon(self.organs.leftHand.holdedItem.sprite)
		if self.organs.rightHand.holdedItem:
			self._uiFields.rhand.AddIcon(self.organs.rightHand.holdedItem.sprite)
