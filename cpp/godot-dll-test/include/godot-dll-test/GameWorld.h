#pragma once

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/Godot.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#pragma warning( pop )

#include "ShootableObject.h"
#include "EditorProperties.h"
#include "EditorProperty.h"
#include "property_conversions.h"

namespace godot
{

class InputEventKey;

class GameWorld : public Node2D
{

	// Godot structure
private:
	GDCLASS(GameWorld, Node2D);

	struct GameWorldProps : EditorProperties
	{
		friend class GameWorld;
		GameWorldProps() : EditorProperties({ &astCooldown, &astDistMin, &astDistMax, &gravity }) {}
	private:
		EditorProperty<Cooldown> astCooldown = { "Asteroid cooldown [s]", 6 };
		EditorPropertyDouble astDistMin = { "Min spawn dist", 800 };
		EditorPropertyDouble astDistMax = { "Max spawn dist", 1300 };
		EditorPropertyDouble gravity = { "Gravity multiplier", 100 };
	};

public:
	static void _bind_methods();
	void _init();
	void _ready();
	virtual void _process(double delta) override;
	virtual void _physics_process(double delta) override;

	GameWorld();
	~GameWorld();

private:
	GameWorldProps props;
	ShootableObject* asteroidTemplate = nullptr;
	RandomNumberGenerator gen;

public:
#pragma region editor properties
	bool _set(const StringName& p_name, const Variant& p_value) { return props.set(p_name, p_value); }
	bool _get(const StringName& p_name, Variant& r_ret) const { return props.get(p_name, r_ret); }
	void _get_property_list(List<PropertyInfo>* p_list) const { props.getPropertyList(p_list); }
	bool _property_can_revert(const StringName& p_name) const { return props.canRevert(p_name); }
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const { return props.getRevert(p_name, r_property); }
#pragma endregion
};
}