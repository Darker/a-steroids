#pragma once

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/variant/vector2.hpp>
#pragma warning( pop )

struct Movement2D
{
  Movement2D(double x, double y, double rads) : linearDelta(x,y), rotationDelta(rads) {}
  Movement2D(const godot::Vector2& velocity, double rads) : linearDelta(velocity), rotationDelta(rads) {}
  double rotationDelta;
  godot::Vector2 linearDelta;
};