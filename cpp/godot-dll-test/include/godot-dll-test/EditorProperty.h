#pragma once

#include "EditorPropertyBase.h"

template <typename T>
struct EditorProperty : EditorPropertyBase
{
  static Variant toVariant(const T& val) { return Variant(val); }
  static T fromVariant(const Variant& val) { return (T)val; }
  static void assignFromVariant(const Variant& src, T& tgt) { tgt = (T)src; }

  EditorProperty(const StringName& name, T value, T defaultValue)
    : EditorPropertyBase(name)
    , value(value)
    , defaultValue(defaultValue)
    , cachedType(toVariant(value).get_type())
  {}

  EditorProperty(const StringName& name, T value)
    : EditorPropertyBase(name)
    , value(value)
    , defaultValue(value)
    , cachedType(toVariant(value).get_type())
  {}
 
  #pragma region cast to contained type
  const T& val() const { return value; }
  T& val() { return value; }
  operator const T& () const { return value; }
  operator T& () { return value; }
  T* operator->() { return &value; }
  const T* operator->() const { return &value; }
  #pragma endregion

  virtual bool set(const Variant& newValue) override { assignFromVariant(newValue, value); return true; }
  virtual Variant get() const override { return toVariant(value); }
  virtual Variant getDefault(bool& ok) const override
  {
    ok = true;
    return toVariant(defaultValue);
  }

  virtual Variant::Type getType() const override { return cachedType; }
  virtual bool hasDefault() const override { return true; }
  virtual void revert() override { value = defaultValue; }

protected:
  T defaultValue;
  T value;
  Variant::Type cachedType;
};

using EditorPropertyDouble = EditorProperty<double>;
using EditorPropertyFloat = EditorProperty<float>;
using EditorPropertyInt32 = EditorProperty<int>;
