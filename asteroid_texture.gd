extends ShootableObject

func _process(delta):
	process(delta)

var textureIndex = {}

func _on_asteroid_health_changed():
	
	if textureIndex.size() == 0:
		for child in self.get_children():
			if child is Sprite2D and child.has_meta("health"):
				textureIndex[child.get_meta("health")] = child
	
	var closest;
	var closestDist = 1000000000;
	var myHealth = getHealthPercent() * 100
	print("Current: ", myHealth)
	for health in textureIndex:
		var dist = abs(myHealth - health);
		print("  ", textureIndex[health].get_name(),"ideal:", health, "dist:",dist)
		if dist < closestDist:
			closest = textureIndex[health]
			closestDist = dist
		textureIndex[health].set_visible(false)
	closest.set_visible(true)
	
	
