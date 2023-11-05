#pragma once
#include <EditorProperties.h>

#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/templates/list.hpp>

class EditorPropertiesMixin
{
public:
	EditorPropertiesMixin(EditorProperties& props) : _mprops{props} {}

#pragma region editor properties
	bool _set(const StringName& p_name, const Variant& p_value) { return _mprops.set(p_name, p_value); }
	bool _get(const StringName& p_name, Variant& r_ret) const { return _mprops.get(p_name, r_ret); }
	void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const { _mprops.getPropertyList(p_list); }
	bool _property_can_revert(const StringName& p_name) const { return _mprops.canRevert(p_name); }
	bool _property_get_revert(const StringName& p_name, Variant& r_property) const { return _mprops.getRevert(p_name, r_property); }
#pragma endregion
private:
	EditorProperties& _mprops;
};
