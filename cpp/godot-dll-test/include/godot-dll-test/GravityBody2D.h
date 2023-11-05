#pragma once

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/Godot.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#pragma warning( pop )

#include "EditorProperties.h"
#include "EditorProperty.h"
#include "Cooldown.h"

namespace godot
{
class GravityBody2D : public RigidBody2D
{
	// Godot structure
private:
	GDCLASS(GravityBody2D, RigidBody2D);

	struct Props : EditorProperties
	{
		EditorPropertyBool gravitySrc{ "Exerts gravity", false };
		EditorPropertyBool gravityTgt{ "Experiences gravity", false };

		Props() : EditorProperties({ &gravitySrc, &gravityTgt }) {}
	};
public:
	static void _bind_methods();
	void _init();
	void _ready();
	void _process(double delta);
	virtual void _physics_process(double delta) override;

	GravityBody2D();
	~GravityBody2D();

	void resetGravity() { gravityVector = Vector2{ 0,0 }; }
	void addGravityInteraction(GravityBody2D* other, real_t gravityConstant = 6.6743e-11f);
	

#pragma region editor properties
	bool _set(const StringName& p_name, const Variant& p_value) { return props.set(p_name, p_value); }
	bool _get(const StringName& p_name, Variant& r_ret) const { return props.get(p_name, r_ret); }
	void _get_property_list(List<PropertyInfo>* p_list) const { props.getPropertyList(p_list); }
	bool _property_can_revert(const StringName& p_name) const { return props.canRevert(p_name); }
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const { return props.getRevert(p_name, r_property); }
#pragma endregion

protected:
	Props props;
	Vector2 gravityVector{ 0,0 };
	// returns 1 for "no gravity" because this essentially removes this object's mass from the gravity formula
	double gravityMassMultiplier() const { return props.gravitySrc ? get_mass() : 1.0; }
};
}