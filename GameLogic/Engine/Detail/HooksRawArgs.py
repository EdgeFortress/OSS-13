from Engine.Hooks import *
from Engine.Server import Player
from Engine.Map import FillMap
from Engine.World import Map

def rawOnPlayerJoined(player):
	OnPlayerJoined(Player(player))

def rawFillMap(map):
	FillMap(Map(map))
