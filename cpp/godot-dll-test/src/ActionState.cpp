#include "ActionState.h"
#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/classes/input.hpp>
#pragma warning( pop )

bool ActionState::isActive() const
{
  if (actionName.empty())
  {
    return false;
  }

  const godot::Input* i = godot::Input::get_singleton();
  const bool pressed = i->is_action_pressed(actionName.c_str());
  return pressed != inverse;
}

bool ActionState::isValid() const
{
  return actionName.empty();
}