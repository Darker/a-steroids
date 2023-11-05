#pragma warning( push )
#pragma warning( disable : 4244)
#include <gdextension_interface.h>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#pragma warning( pop )

#include "Player.h"
#include "Projectile.h"
#include "ShootableObject.h"
#include "ShipGun.h"
#include "GameWorld.h"
#include "GravityBody2D.h"

void initialize_example_module(godot::ModuleInitializationLevel p_level)
{
	if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
	godot::ClassDB::register_class<godot::Player>();
	godot::ClassDB::register_class<godot::Projectile>();
	godot::ClassDB::register_class<godot::ShootableObject>();
	godot::ClassDB::register_class<godot::ShipGun>();
	godot::ClassDB::register_class<godot::GameWorld>();
	godot::ClassDB::register_class<godot::GravityBody2D>();
}

void uninitialize_example_module(godot::ModuleInitializationLevel p_level)
{
	if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
}

//extern "C" {
//
//	// Initialization.
//
//	GDNativeBool GDN_EXPORT example_library_init(const GDNativeInterface* p_interface, const GDNativeExtensionClassLibraryPtr p_library, GDNativeInitialization* r_initialization)
//	{
//		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);
//
//		init_obj.register_initializer(initialize_example_module);
//		init_obj.register_terminator(uninitialize_example_module);
//		init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);
//
//		return init_obj.init();
//	}
//}

//extern "C" {
//	// Initialization.
//	GDExtensionBool GDE_EXPORT example_library_init(const GDExtensionInterface* p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
//	{
//		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);
//
//		init_obj.register_initializer(initialize_example_module);
//		init_obj.register_terminator(uninitialize_example_module);
//		init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);
//
//		return init_obj.init();
//	}
//}

extern "C" {
	// Initialization.
	GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_example_module);
		init_obj.register_terminator(uninitialize_example_module);
		init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}