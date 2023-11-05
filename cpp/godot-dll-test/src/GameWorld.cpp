#include "GameWorld.h"
#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/collision_polygon2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#pragma warning( pop )

#include "nodes_util.h"
#include "Player.h"
#include "GravityBody2D.h"
#include "math_util.h"

namespace godot
{

void GameWorld::_bind_methods()
{

}

GameWorld::GameWorld()
{

}
GameWorld::~GameWorld()
{

}

void GameWorld::_process(double delta)
{
	Engine* eng = Engine::get_singleton();
	if (eng->is_editor_hint())
	{
		return;
	}
	// generate sample boulders
	if (asteroidTemplate == nullptr)
	{
		asteroidTemplate = findChild<ShootableObject>(this, "asteroid_template");
	}

	if (asteroidTemplate != nullptr)
	{
		if (props.astCooldown->trigger(delta))
		{
			const Player* player = findChild<Player>(this, "player");
			const auto playerPos = player->get_position();
			const auto playerSpeed = player->get_linear_velocity();
			const auto offsetAngleVector = Vector2::from_angle((float)gen.randf_range(0.0, PI * 2));
			const auto spawnPoint = playerPos + offsetAngleVector * gen.randf_range(props.astDistMin, props.astDistMax);


			ShootableObject* asteroid = (ShootableObject*)asteroidTemplate->duplicate(DUPLICATE_SIGNALS & DUPLICATE_SCRIPTS);
			add_child(asteroid);
			asteroid->set_owner(this);
			asteroid->set_position(spawnPoint);
			const auto rotateAttackAngle = gen.randf_range(-0.005, 0.005);
			const auto randomSpeed = gen.randf_range(5, 40);
			asteroid->set_linear_velocity((-offsetAngleVector).rotated(rotateAttackAngle) * randomSpeed + playerSpeed * 0.5);
			asteroid->set_freeze_enabled(false);
			asteroid->set_visible(true);
			asteroid->set_angular_velocity(gen.randf_range(0, PI * 0.5));
			findChild<CollisionPolygon2D>(asteroid, "collision")->set_disabled(false);
		}
	}
	else
	{
		UtilityFunctions::print("Failed to find asteroid template!");
	}
}

void GameWorld::_init() {}

void godot::GameWorld::_ready()
{

}

GravityBody2D* getValidGBody(Node* parent, int32_t idx)
{
	Node* ach = parent->get_child(idx);
	if (ach->is_inside_tree())
	{
		return godot::Object::cast_to<GravityBody2D>(ach);
	}
	return nullptr;
}

void GameWorld::_physics_process(double delta)
{
	const auto count = get_child_count();
	bool first = true;
	for (int32_t i = 0; i < count; ++i)
	{
		if (GravityBody2D* left = getValidGBody(this, i))
		{
			if (first)
			{
				left->resetGravity();
				first = false;
			}

			for (auto j = i + 1; j < count; ++j)
			{
				if (GravityBody2D* right = getValidGBody(this, j))
				{
					right->resetGravity();
					left->addGravityInteraction(right, props.gravity.getValue());
				}
			}
		}
	}
}

}
