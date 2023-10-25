#pragma once

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/Godot.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#pragma warning( pop )

#include "EditorProperties.h"
#include "EditorProperty.h"

namespace godot
{

class Projectile : public RigidBody2D
{
	// Godot structure
private:
	GDCLASS(Projectile, RigidBody2D);

	struct Props : EditorProperties
	{
		EditorPropertyDouble damage = { "Damage", 9 };
		EditorPropertyDouble maxLifetime = { "Max lifetime [s]", 25 };
		Props() : EditorProperties({&damage, &maxLifetime}) {}
	};

public:
	static void _bind_methods();
	void _init();
	void _ready();
	virtual void _process(double delta) override;
	virtual void _physics_process(double delta) override;

	Projectile();
	~Projectile();

  #pragma region editor properties
	bool _set(const StringName& p_name, const Variant& p_value) { return props.set(p_name, p_value); }
	bool _get(const StringName& p_name, Variant& r_ret) const { return props.get(p_name, r_ret); }
	void _get_property_list(List<PropertyInfo>* p_list) const { props.getPropertyList(p_list); }
	bool _property_can_revert(const StringName& p_name) const { return props.canRevert(p_name); }
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const { return props.getRevert(p_name, r_property); }
  #pragma endregion

private:
	Props props;
	double lifetime = 0;

};
}