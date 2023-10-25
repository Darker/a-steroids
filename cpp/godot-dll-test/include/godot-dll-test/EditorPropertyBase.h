#pragma once
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string_name.hpp>

using godot::Variant;
using godot::StringName;

struct EditorPropertyBase
{
  EditorPropertyBase(const StringName& name) : name(name) {}

  virtual bool set(const Variant& value) = 0;
  virtual Variant get() const = 0;
  virtual Variant getDefault(bool& ok) const = 0;
  virtual Variant::Type getType() const = 0;
  virtual void revert() = 0;
  virtual bool hasDefault() const = 0;
  Variant getDefault() const { bool unused; return getDefault(unused); }
  const StringName& getName() const { return name; }

protected:
  StringName name;
};