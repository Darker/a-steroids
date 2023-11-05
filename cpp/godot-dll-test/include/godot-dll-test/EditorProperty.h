#pragma once
#include "EditorPropertyBase.h"
#include "EditorPropertyRange.h"

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/variant/utility_functions.hpp>
#pragma warning( pop )

#include <type_traits>
#include <cmath>


template <typename T>
struct EditorProperty : EditorPropertyBase
{
  static Variant toVariant(const T& val) { return Variant(val); }
  static bool assignFromVariant(const Variant& src, T& tgt) { tgt = (T)src; return true; }
  static T fromVariant(const Variant& val) { T res{}; assignFromVariant(val, res); return res; }

  EditorProperty(const StringName& name, T initVal, T defaultVal, EditorPropertyRange range)
    : EditorPropertyBase(name)
    , value{ initVal }
    , defaultValue{ defaultVal }
    , range{ range }
    , cachedType(toVariant(value).get_type())
  {}

  EditorProperty(const StringName& name, T value, T defaultValue)
    : EditorProperty(name, value, defaultValue, EditorPropertyRange{})
  {}

  EditorProperty(const StringName& name, T value)
    : EditorProperty(name, value, value, EditorPropertyRange{})
  {}

  EditorProperty(const StringName& name, T value, EditorPropertyRange range)
    : EditorProperty(name, value, value, range)
  {}
 
  #pragma region cast to contained type
  const T& val() const { return value; }
  T& val() { return value; }
  operator const T& () const { return value; }
  operator T& () { return value; }
  T* operator->() { return &value; }
  const T* operator->() const { return &value; }
  T& getValue() { return value; }
  const T& getValue() const { return value; }
  #pragma endregion

  virtual bool set(const Variant& newValue) override { return assignFromVariant(newValue, value); }
  virtual Variant getVariant() const override { return toVariant(value); }
  virtual Variant getDefault(bool& ok) const override
  {
    ok = true;
    return toVariant(defaultValue);
  }

  virtual Variant::Type getType() const override { return cachedType; }
  virtual bool hasDefault() const override { return true; }
  virtual void revert() override { value = defaultValue; }

  virtual EditorPropertyHint getHint() const override
  {
    EditorPropertyHint hint{};
    godot::UtilityFunctions::print("Range valid: ", range.isValid());
    if (range.isValid())
    {
      hint.hintType = godot::PROPERTY_HINT_RANGE;
      hint.hintString = (godot::String("%f,") % range.min)
        + (godot::String("%f,") % range.max)
        + (godot::String("%f,") % range.step)
        + "or_less,or_greater";
      //hint.hintString = hint.hintString % range.min % range.max % range.step;
      if (range.exp)
      {
        hint.hintString += ",exp";
      }
      if (!range.suffix.is_empty())
      {
        hint.hintString += ",suffix:";
        hint.hintString += range.suffix;
      }
    }
    return hint;
  }

protected:
  T defaultValue;
  T value;
  Variant::Type cachedType;
  EditorPropertyRange range;
};

using EditorPropertyDouble = EditorProperty<double>;
using EditorPropertyFloat = EditorProperty<float>;
using EditorPropertyReal = EditorProperty<real_t>;
using EditorPropertyInt32 = EditorProperty<int>;
using EditorPropertyBool = EditorProperty<bool>;
