from Engine import Map, CreateObject, Vec3i
from Objects.Turfs.Airlock import Airlock

def FillMap(map):
	for i in range(45, 56):
		for j in range(45, 56):
			CreateObject("Objects.Turfs.Floor", map.GetTile(Vec3i(i, j, 0)))
			if i == 45 or i == 55 or j == 45 or j == 55:
				if i == 50 or j == 50:
					airlock = CreateObject("Objects.Turfs.Airlock", map.GetTile(Vec3i(i, j, 0)))
					if (i == 55 and j == 50) or (i == 50 and j == 55):
						airlock.Lock()
				else:
					CreateObject("Objects.Turfs.Wall", map.GetTile(Vec3i(i, j, 0)))
	CreateObject("Objects.Items.Taser", map.GetTile(Vec3i(47, 53, 0)))
	CreateObject("Objects.Items.Taser", map.GetTile(Vec3i(53, 47, 0)))



