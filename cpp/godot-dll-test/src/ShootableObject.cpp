#include "ShootableObject.h"
#include "nodes_util.h"
#include "math_util.h"
#include <godot_cpp/classes/sprite2d.hpp>

#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/classes/collision_polygon2d.hpp>
#include <godot_cpp/classes/polygon2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#pragma warning( pop )


namespace godot
{
void ShootableObject::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("getHealth"), &ShootableObject::getHealth);
  ClassDB::bind_method(D_METHOD("getHealthPercent"), &ShootableObject::getHealthPercent);
  ClassDB::bind_method(D_METHOD("handleDeath"), &ShootableObject::handleDeath);
  ADD_SIGNAL(MethodInfo("healthChanged"));
  ClassDB::bind_method(D_METHOD("emitHealthChanged"), &ShootableObject::emitHealthChanged);


  ClassDB::bind_method(D_METHOD("process"), &ShootableObject::process);
}

void ShootableObject::_init()
{}

void ShootableObject::_ready()
{
  set_angular_damp(0.0);
  set_linear_damp(0);

  health = props.maxHealth;
  lastHealth = health;
  gen.randomize();
  //UtilityFunctions::print("Print children:");
  //print_tree();
  //UtilityFunctions::print("Number of children as listed above: ", get_child_count());
  if (scaleOverride != 1.0)
  {
    for (int i = 0; i < get_child_count(); ++i)
    {
      Node* child = get_child(i);
      if (Node2D* body = Object::cast_to<Node2D>(child))
      {
        body->set_scale(body->get_scale() * scaleOverride);
        //UtilityFunctions::print("Child node name: "+body->get_name()+" type: "+body->get_class());
        //if (collisionOverride.size() > 0)
        //{
        //  if (CollisionPolygon2D* collision = Object::cast_to<CollisionPolygon2D>(body))
        //  {
        //    collision->set_polygon(collisionOverride);
        //  }
        //}
      }
    }
  }

  ifChild<Sprite2D>(this, "center_of_mass_indicator", [this](Sprite2D* sprite)
  {
    sprite->set_position(get_center_of_mass());
  });
  
  if (collisionOverride.size() > 0)
  {
    //CollisionPolygon2D* collision = findChild<CollisionPolygon2D>(this, "collision");
    CollisionPolygon2D* collision = Object::cast_to<CollisionPolygon2D>(find_child("collision"));
    if (collision)
    {
      collision->set_polygon(collisionOverride);
      findChild<Polygon2D>(this, "clip_polygon")->set_polygon(collisionOverride);

    }
    else
    {
      UtilityFunctions::print("Cannot override collision, node not found! Trying alternative method");
      for (int i = 0; i < get_child_count(); ++i)
      {
        Node* child = get_child(i);
        if (collision = checkChild<CollisionPolygon2D>(child, "collision"))
        {
          collision->set_polygon(collisionOverride);
          UtilityFunctions::print("Collision set");
        }
        if (Polygon2D* polygon = checkChild<Polygon2D>(child, "clip_polygon"))
        {
          polygon->set_polygon(collisionOverride);
          UtilityFunctions::print("Polygon set");
        }
      }
    }
  }

  // this wont work, because, well godot
  //// copy scale to children
  //TypedArray<Node2D> children = get_children(true);
  //for (int i = 0; i < children.size(); ++i)
  //{
  //  Object* o = children[i];
  //  if (Node2D* body = Object::cast_to<Node2D>(o))
  //  {
  //    body->set_scale(body->get_scale() * scaleOverride);
  //  }
  //}
}

void ShootableObject::_process(double delta)
{
  process(delta);
}

void ShootableObject::_physics_process(double delta)
{}

void ShootableObject::process(double delta)
{
  if (health <= 0.0)
  {
    handleDeath();
  }
  else
  {
    health += 0.0001 * props.maxHealth;
  }
}

bool ShootableObject::receiveDamage(double damage)
{
  lastHealth = health;
  health -= damage;
  if (health < 0.0)
    health = 0;
  emitHealthChanged();

  return health == 0.0;
}

double ShootableObject::getHealth() const
{
  return health;
}

ShootableObject::ShootableObject()
{}

ShootableObject::~ShootableObject()
{}

void ShootableObject::handleDeath()
{

  //createCircularFragments();
  createShardFragments();
  queue_free();
}

void ShootableObject::createShardFragments()
{
  // find collision path
  const CollisionPolygon2D* collision = findChild<CollisionPolygon2D>(this, "collision");
  if (collision)
  {
    const auto origPath = collision->get_polygon();
    const auto origLen = origPath.size();
    const auto origArea = polygonArea(origPath);
    if (origLen > 5)
    {
      const auto lenThird = origPath.size() / 3;
      // try to split it
      PackedVector2Array path1;
      PackedVector2Array path2;
      //const auto splitLen = gen.randi_range(lenThird, origLen - lenThird);
      //gen.randomize();
      const auto splitLen = origPath.size() / 2;

      for (int i = 0; i < origLen; ++i)
      {
        if (i < splitLen)
        {
          path1.append(origPath[i]);
        }
        else
        {
          path2.append(origPath[i]);
        }
      }

      
      std::array<PackedVector2Array*, 2> paths {&path1, &path2};

      // add 2 interpolated points between start and end
      for (auto* path : paths)
      {
        auto& pref = *path;
        auto end = pref[pref.size() - 1];
        const auto dist = end.distance_to(pref[0]);
        pref.append(end.move_toward(pref[0], dist * 0.33333));
        pref.append(end.move_toward(pref[0], dist * 0.66666));
      }
      
      for (const auto* path : paths)
      {
        const auto shardArea = polygonArea(*path);
        if (shardArea > 4)
        {
          ShootableObject* myCopy = (ShootableObject*)duplicate(DUPLICATE_SIGNALS & DUPLICATE_SCRIPTS);
          myCopy->scaleOverride = 1;

          const auto sizeRatio = shardArea / origArea;
          myCopy->props.maxHealth.val() = props.maxHealth.val() * sizeRatio * 1.1;
          myCopy->set_mass(get_mass() * sizeRatio);
          myCopy->collisionOverride = *path;

          // recalculate center point
          Vector2 centroid = polygonCentroid(*path);
          // move in the direction of the new center
          myCopy->set_linear_velocity(centroid * 0.8 + get_linear_velocity());
          myCopy->set_center_of_mass_mode(RigidBody2D::CenterOfMassMode::CENTER_OF_MASS_MODE_CUSTOM);
          myCopy->set_center_of_mass(centroid);

          get_parent()->add_child(myCopy);
          myCopy->set_owner(get_parent());
        }
        else
        {
          UtilityFunctions::print("Final path has small area, will not split.");
        }
      }
    }
    else
    {
      UtilityFunctions::print("Final path too small, will not split.");
    }
  }
  else
  {
    UtilityFunctions::print("Cannot split, collision node missing.");
  }
}

void ShootableObject::createCircularFragments()
{
  // split this into bunch of new asteroids
  const double origMass = get_mass();
  double availableMass = origMass * 0.5;
  const double minMass = availableMass / 6.0;
  const double maxMass = availableMass / 1.8;
  const double healthPerKG = props.maxHealth.val() / origMass;
  const Vector2 scalePerKG = get_scale() / origMass;

  Vector2 baseVelocity = get_linear_velocity();
  double baseSpeed = baseVelocity.length();

  while (availableMass > minMass)
  {
    const double mass = gen.randf_range(minMass, maxMass);
    availableMass -= mass;
    gen.randomize();
    // there is additional health for each chunk
    const auto health = healthPerKG * mass * 1.2;

    if (health > 1.0)
    {
      ShootableObject* myCopy = (ShootableObject*)duplicate(DUPLICATE_SIGNALS & DUPLICATE_SCRIPTS);

      double speed = gen.randf_range(-20, 20) + baseSpeed;
      gen.randomize();
      Vector2 newVelocity = { (real_t)speed, 0 };
      newVelocity = newVelocity.rotated(gen.randf_range(0, 2 * 3.14159));
      gen.randomize();
      // move a lil bit
      myCopy->set_position(myCopy->get_position() + newVelocity * 0.4);


      myCopy->set_mass(mass);

      double scaleRatio = mass / origMass;
      //scaleRatio = (Math::sqrt(scaleRatio) + scaleRatio) / 2.0;
      if (props.squareScale)
      {
        scaleRatio = Math::sqrt(scaleRatio);
      }

      myCopy->scaleOverride = scaleRatio;
      //myCopy->set_scale(scalePerKG * mass * 0.1);
      myCopy->props.maxHealth.val() = health;


      myCopy->set_linear_velocity(newVelocity);
      get_parent()->add_child(myCopy);
      myCopy->set_owner(get_parent());
    }
  }
}

}
