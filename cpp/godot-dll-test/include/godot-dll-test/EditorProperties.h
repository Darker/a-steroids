#pragma once
#include "EditorPropertyBase.h"
#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/templates/list.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#pragma warning( pop )
#include <vector>
#include <memory>



struct EditorProperties
{
public:
  EditorProperties(std::initializer_list<EditorPropertyBase*> l)
  {
    props.insert(props.end(), l);
  }

  void getPropertyList(godot::List<godot::PropertyInfo>* p_list) const
  {
    for (const auto* prop : props)
    {
      const auto hint = prop->getHint();
      if (hint.hintType != godot::PROPERTY_HINT_NONE)
      {
        godot::UtilityFunctions::print("Property hint: ", hint.hintString.ascii().get_data());
      }
      
      p_list->push_back(godot::PropertyInfo(prop->getType(), prop->getName(), hint.hintType, hint.hintString));
      //if (hint.hintType != godot::PROPERTY_HINT_NONE)
      //{
      //  p_list->push_back(godot::PropertyInfo(prop->getType(), prop->getName(), hint.hintType, hint.hintString));
      //}
      //else
      //{
      //  p_list->push_back(godot::PropertyInfo(prop->getType(), prop->getName()));
      //}
    }
  }

  bool get(const StringName& name, Variant& tgt) const
  {
    if (const EditorPropertyBase* prop = find(name))
    {
      tgt = prop->getVariant();
      return true;
    }

    return false;
  }

  bool set(const StringName& name, const Variant& v)
  {
    if (EditorPropertyBase* prop = find(name))
    {
      return prop->set(v);
    }

    return false;
  }

  bool canRevert(const StringName& name) const
  {
    if (const EditorPropertyBase* prop = find(name))
    {
      return prop->hasDefault();
    }

    return false;
  }

  bool getRevert(const StringName& name, Variant& tgt) const
  {
    if (const EditorPropertyBase* prop = find(name))
    {
      if (prop->hasDefault())
      {
        tgt = prop->getDefault();
        return true;
      }
    }

    return false;
  }

private:
  std::vector<EditorPropertyBase*> props;

  const EditorPropertyBase* find(const StringName& name) const
  {
    for (const auto* prop : props)
    {
      if (prop->getName() == name)
      {
        return prop;
      }
    }
    return nullptr;
  }
  EditorPropertyBase* find(const StringName& name)
  {
    return const_cast<EditorPropertyBase*>(const_cast<const EditorProperties*>(this)->find(name));
  }
};