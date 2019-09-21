from Engine.World import Map, CreateObject, Object
from Engine.Geometry import Vector
from Objects.Turfs.Airlock import Airlock

def FillMap(map):
	"""
	FillMap is called by Engine when map is created

	You can add any map generation logic here

	"""

	for i in range(45, 56):
		for j in range(45, 56):
			CreateObject("Objects.Turfs.Floor.Floor", map.GetTile(Vector(i, j, 0)))
			if i == 45 or i == 55 or j == 45 or j == 55:
				if i == 50 or j == 50:
					airlock = CreateObject("Objects.Turfs.Airlock.Airlock", map.GetTile(Vector(i, j, 0)))
					if (i == 55 and j == 50) or (i == 50 and j == 55):
						airlock.Lock()
				else:
					CreateObject("Objects.Turfs.Wall.Wall", map.GetTile(Vector(i, j, 0)))
	CreateObject("Objects.Items.Taser.Taser", map.GetTile(Vector(47, 53, 0)))
	CreateObject("Objects.Items.Taser.Taser", map.GetTile(Vector(53, 47, 0)))
