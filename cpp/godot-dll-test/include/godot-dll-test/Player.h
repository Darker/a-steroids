#pragma once

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/Godot.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/input.hpp>
#pragma warning( pop )

#include "Cooldown.h"
#include "Movement2D.h"
#include "EditorProperties.h"
#include "EditorProperty.h"

template<>
inline void EditorProperty<Cooldown>::assignFromVariant(const godot::Variant& src, Cooldown& tgt)
{
	tgt.setDuration(EditorPropertyDouble::fromVariant(src));
}
template<>
inline Variant EditorProperty<Cooldown>::toVariant(const Cooldown& tgt)
{
	return EditorPropertyDouble::fromVariant(tgt.getDuration());
}

namespace godot
{

class InputEventKey;

class Player : public RigidBody2D
{

	// Godot structure
private:
	GDCLASS(Player, RigidBody2D);

	struct PlayerProps : EditorProperties
	{
		friend class Player;
		PlayerProps() : EditorProperties({&acceleration, &angularAcceleration, &shootCooldown}) {}
	private:
		EditorPropertyDouble acceleration = { "acceleration", 30 };
		EditorPropertyDouble angularAcceleration = { "angular_acceleration", 2000 };
		EditorProperty<Cooldown> shootCooldown = { "Shoot cooldown [s]", 0.5 };
	};

public:
	static void _bind_methods();
	void _init();
	void _ready();
	virtual void _process(double delta) override;
	virtual void _physics_process(double delta) override;

	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name, Variant& r_ret) const;
	void _get_property_list(List<PropertyInfo>* p_list) const;
	bool _property_can_revert(const StringName& p_name) const;
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const;

	Player();
	~Player();

	// Gameplay variables
private:
	Ref<InputEventKey> accelerate;
	PlayerProps props;

	// Gameplay methods
public:
	Movement2D get_movement_delta();

private:




};
}