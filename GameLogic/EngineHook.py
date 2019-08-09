from Objects.Creatures.Ghost import Ghostize 

def OnPlayerJoined(player):
	print(player.ckey + " has joined! Yay!")
	player.AddVerb("ghost", Ghostize)
