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
#include "GravityBody2D.h"

namespace godot
{
class ShootableObject : public GravityBody2D
{
	// Godot structure
private:
	GDCLASS(ShootableObject, GravityBody2D);

	struct Props : EditorProperties
	{
		EditorPropertyDouble maxHealth{ "Max health", 100 };
		EditorPropertyBool squareScale{ "Enlarge fragments ", true };
		EditorPropertyDouble healthMultiplier{ "Fragment HP mult", 1.2 };
		Props() : EditorProperties({&maxHealth, &squareScale, &healthMultiplier}) {}
	};
public:
	static void _bind_methods();
	void _init();
	void _ready();
	void _process(double delta);
	virtual void _physics_process(double delta) override;

	virtual void process(double delta);
	/// <summary>
	/// Damages this object and returns true if its health is now zero
	/// </summary>
	/// <param name="damage">Amount of damage</param>
	/// <returns>true if dead</returns>
	virtual bool receiveDamage(double damage);
	virtual double getHealth() const;
	virtual double getHealthPercent() const { return getHealth() / props.maxHealth.val(); }
	virtual void heal() { lastHealth = health; health = props.maxHealth.val(); }
	void emitHealthChanged() { emit_signal("healthChanged"); }
	
	ShootableObject();
	~ShootableObject();

  #pragma region editor properties
	bool _set(const StringName& p_name, const Variant& p_value) { return props.set(p_name, p_value); }
	bool _get(const StringName& p_name, Variant& r_ret) const { return props.get(p_name, r_ret); }
	void _get_property_list(List<PropertyInfo>* p_list) const { props.getPropertyList(p_list); }
	bool _property_can_revert(const StringName& p_name) const { return props.canRevert(p_name); }
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const { return props.getRevert(p_name, r_property); }
  #pragma endregion

protected:
	Props props;
	double health;
	double lastHealth;
	double scaleOverride = 1.0;
	// this is set to override original collision shape on a fragmented asteroid
	PackedVector2Array collisionOverride;
	// after this time, all collision exceptions are removed
	Cooldown stopIgnoringCollisions = { 0.8 };
	virtual void handleDeath();
	void createCircularFragments();
	void createShardFragments();
	RandomNumberGenerator gen;

};
}