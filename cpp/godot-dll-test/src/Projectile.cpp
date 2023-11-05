#include "Projectile.h"
#include "ShootableObject.h"
#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/variant/utility_functions.hpp>
#pragma warning( pop )

namespace godot
{
void Projectile::_bind_methods()
{}

void Projectile::_init()
{}

void Projectile::_ready()
{
  set_contact_monitor(true);
  set_max_contacts_reported(2);
}

void Projectile::_process(double delta)
{
  set_angular_damp(0.0);
  set_linear_damp(0);

  lifetime += delta;
  if (lifetime >= props.maxLifetime.val())
  {
    queue_free();
  }
}

void Projectile::_physics_process(double delta)
{
  TypedArray<Node2D> colliders = get_colliding_bodies();
  bool hit = false;
  for (int i=0; i<colliders.size(); ++i)
  {
    Object* o = colliders[i];

    if (PhysicsBody2D* body = Object::cast_to<PhysicsBody2D>(o))
    {
      if (ShootableObject* shootable = Object::cast_to<ShootableObject>(body))
      {
        //UtilityFunctions::print(body->get_class());
        //if (body->get_class() != "ShootableObject")
        //{
        //  return;
        //}

        if(shootable->is_inside_tree())
          shootable->receiveDamage(props.damage);
      }
      hit = true;
    }
  }

  if (hit)
  {
    queue_free();
  }
}

Projectile::Projectile()
{}

Projectile::~Projectile()
{}

}
