#pragma once
#include "EditorProperty.h"
#include "Cooldown.h"

template<>
inline bool EditorProperty<Cooldown>::assignFromVariant(const godot::Variant& src, Cooldown& tgt)
{
  if (godot::Variant::can_convert(src.get_type(), godot::Variant::FLOAT))
  {
    tgt.setDuration(EditorPropertyDouble::fromVariant(src));
    return true;
  }
  return false;
}

template<>
inline Variant EditorProperty<Cooldown>::toVariant(const Cooldown& tgt)
{
  return EditorPropertyDouble::fromVariant(tgt.getDuration());
}
