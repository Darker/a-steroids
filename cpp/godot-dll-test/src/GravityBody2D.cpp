#include "GravityBody2D.h"
#include "nodes_util.h"
#include "math_util.h"

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/variant/utility_functions.hpp>
#pragma warning( pop )


namespace godot
{
void GravityBody2D::_bind_methods()
{

}

void GravityBody2D::_init()
{}

void GravityBody2D::_ready()
{

}

void GravityBody2D::_process(double delta)
{

}

void GravityBody2D::addGravityInteraction(GravityBody2D* other, real_t gravityConstant)
{
  const auto myPos = get_position();
  const auto otherPos = other->get_position();
  const auto distanceSq = myPos.distance_squared_to(other->get_position());

  const auto myMass = gravityMassMultiplier();
  const auto otherMass = other->gravityMassMultiplier();
  
  const auto magnitude = gravityConstant * myMass * otherMass / distanceSq;
  const auto myVec = Vector2{ static_cast<real_t>(magnitude), 0 }.rotated(myPos.angle_to_point(otherPos));

  if (props.gravityTgt)
  {
    gravityVector += myVec;
  }
  if (other->props.gravityTgt)
  {
    other->gravityVector -= myVec;
  }
}

void GravityBody2D::_physics_process(double delta)
{
  apply_central_force(gravityVector);
}

GravityBody2D::GravityBody2D()
{}

GravityBody2D::~GravityBody2D()
{}

}
