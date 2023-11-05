#pragma once

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/Godot.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#pragma warning( pop )

#include "EditorProperties.h"
#include "EditorProperty.h"
#include "EditorPropertiesMixin.h"
#include "Cooldown.h"
#include "property_conversions.h"

namespace godot
{

class ShipGun : public Node2D
{
	// Godot structure
private:
	GDCLASS(ShipGun, Node2D);

	struct Props : EditorProperties
	{
		EditorProperty<Cooldown> shootCooldown = { "Shoot cooldown [s]", 0.5 };
		Props() : EditorProperties({ &shootCooldown }) {}
	};

public:
	static void _bind_methods();
	void _init();
	void _ready();
	virtual void _process(double delta) override;
	virtual void _physics_process(double delta) override;

	ShipGun();
	~ShipGun();

#pragma region editor properties
	bool _set(const StringName& p_name, const Variant& p_value) { return props.set(p_name, p_value); }
	bool _get(const StringName& p_name, Variant& r_ret) const { return props.get(p_name, r_ret); }
	void _get_property_list(List<PropertyInfo>* p_list) const { props.getPropertyList(p_list); }
	bool _property_can_revert(const StringName& p_name) const { return props.canRevert(p_name); }
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const { return props.getRevert(p_name, r_property); }
#pragma endregion

private:
	Props props;


};
}