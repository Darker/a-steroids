#include "Player.h"


#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#pragma warning( pop )

#include "Projectile.h"
#include "KeyConfig.h"
#include "math_util.h"

using namespace godot;



Player::Player()
{

}

Player::~Player() {}

struct PositionalInfo2D
{
	double rotation;
	Vector2 coords;

	operator Vector2() { return coords; }
};

PositionalInfo2D node_real_pos(Node* node, PhysicsBody2D* realParent)
{
	PositionalInfo2D info;
	// getVariant parent pos
	info.coords = realParent->get_position();


	return info;
}

void Player::_process(double delta)
{
	Engine* eng = Engine::get_singleton();
	if (eng->is_editor_hint())
	{
		return;
	}

	//const Input* i = Input::get_singleton();

	props.shootCooldown->elapse(delta);

	if (!props.shootCooldown->isActive() && KeyConfig::get().SHOOT.isActive())
	{
		//motion = { 0,0 };
		props.shootCooldown->reset();
		//p->add_child(this->get_child)
		Node2D* cannon = Object::cast_to<Node2D>(find_child("cannon", false));
		if (cannon)
		{
			if (Projectile* projPrefab = Object::cast_to<Projectile>(cannon->find_child("projectile", false)))
			{
				Projectile* p = static_cast<Projectile*>(projPrefab->duplicate(0));
				p->set_visible(true);
				p->set_process_mode(Node::PROCESS_MODE_INHERIT);
				p->add_collision_exception_with(this);
				// set rotation and position

				this->get_parent()->add_child(p);// projectile_sprite

				p->set_global_position(cannon->get_global_position());
				p->set_linear_velocity(Vector2{ 0, real_t{-1.0} * p->getInitialSpeed() }.rotated(cannon->get_global_rotation()) + get_linear_velocity());
			}
		}
	}
}

void Player::_init() {}

void godot::Player::_ready()
{
	InputMap* map = InputMap::get_singleton();
	map->load_from_project_settings();
	//map->add_action("cpp_action_shoot");
	//
	//accelerate->set_keycode(Key::KEY_SPACE);
	//map->action_add_event("cpp_action_shoot", accelerate);


	set_angular_damp(0.0);
	set_linear_damp(0);

	Node2D* cannon = Object::cast_to<Node2D>(find_child("cannon", false));
	if (cannon)
	{
		UtilityFunctions::print("Cannon discovered.");
	}
	if (!map->has_action("game_shoot"))
	{
		UtilityFunctions::print("No shoot action set!");
	}
}

void Player::_physics_process(double delta)
{
	Movement2D movement = get_movement_delta();
	apply_central_impulse(movement.linearDelta * delta);
	apply_torque_impulse(movement.rotationDelta * delta);
}

bool godot::Player::_set(const StringName& p_name, const Variant& p_value)
{
	return props.set(p_name, p_value);
}

bool godot::Player::_get(const StringName& p_name, Variant& r_ret) const
{
	return props.get(p_name, r_ret);
}

void godot::Player::_get_property_list(List<PropertyInfo>* p_list) const
{
	props.getPropertyList(p_list);
}

bool godot::Player::_property_can_revert(const StringName & p_name) const
{
	return props.canRevert(p_name);
}

bool godot::Player::_property_get_revert(const StringName& p_name, Variant& r_property) const
{
	return props.getRevert(p_name, r_property);
}

Movement2D Player::get_movement_delta()
{
	Movement2D result(0,0, 0);


	const Input* i = Input::get_singleton();

	double delta_v = 0;
	
	if (i->is_action_pressed("ui_up"))
		delta_v += props.acceleration;
	//if (i->is_action_pressed("ui_down"))
	//	delta_v -= props.acceleration;
	if (i->is_action_pressed("ui_left"))
		result.rotationDelta -= props.angularAcceleration;
	if (i->is_action_pressed("ui_right"))
		result.rotationDelta += props.angularAcceleration;

	const auto angularVelocity = get_angular_velocity();
	if (SIGN(result.rotationDelta) != SIGN(angularVelocity))
	{
		
		result.rotationDelta *= props.counterRotation.val() * (square(Math::abs(angularVelocity/5.0))+1);
		//if (-result.rotationDelta > angularVelocity)
		//{
		//	result.rotationDelta = -angularVelocity;
		//}
	}

	//result.rotationDelta -= SIGN(result.rotationDelta) * angular_dampening;

	double rotation = get_rotation();
	Vector2 newVec(0, -delta_v);
	result.linearDelta = newVec.rotated((float)(rotation));

	return result;
}

void Player::_bind_methods()
{
	//ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &Player::_physics_process);
}