#pragma once
#pragma warning( push )
#pragma warning( disable : 4244)
#include <godot_cpp/variant/packed_vector2_array.hpp>
#pragma warning( pop )

static inline double polygonArea(const godot::PackedVector2Array& vertices)
{
  // Initialize area
  double area = 0.0;
  const auto n = vertices.size();
  if (n < 3)
  {
    return 0.0;
  }
  // Calculate value of shoelace formula
  int j = n - 1;
  for (int i = 0; i < n; ++i)
  {
    area += (vertices[j].x + vertices[i].x) * (vertices[j].y - vertices[i].y);
    j = i;  // j is previous vertex to i
  }

  // Return absolute value
  return abs(area / 2.0);
}

static inline godot::Vector2 polygonCentroid(const godot::PackedVector2Array& vertices)
{
  godot::Vector2 centroid = { 0, 0 };
  real_t signedArea = 0.0;
  real_t x0 = 0.0; // Current vertex X
  real_t y0 = 0.0; // Current vertex Y
  real_t x1 = 0.0; // Next vertex X
  real_t y1 = 0.0; // Next vertex Y
  real_t a = 0.0;  // Partial signed area

  // For all vertices except last
  int i = 0;
  const auto vertexCount = vertices.size();
  for (i = 0; i < vertexCount - 1; ++i)
  {
    x0 = vertices[i].x;
    y0 = vertices[i].y;
    x1 = vertices[i + 1].x;
    y1 = vertices[i + 1].y;
    a = x0 * y1 - x1 * y0;
    signedArea += a;
    centroid.x += (x0 + x1) * a;
    centroid.y += (y0 + y1) * a;
  }

  // Do last vertex separately to avoid performing an expensive
  // modulus operation in each iteration.
  x0 = vertices[i].x;
  y0 = vertices[i].y;
  x1 = vertices[0].x;
  y1 = vertices[0].y;
  a = x0 * y1 - x1 * y0;
  signedArea += a;
  centroid.x += (x0 + x1) * a;
  centroid.y += (y0 + y1) * a;

  signedArea *= 0.5;
  centroid.x /= (6.0 * signedArea);
  centroid.y /= (6.0 * signedArea);

  return centroid;
}

template <typename TNum>
static inline constexpr TNum square(TNum num)
{
  return num * num;
}
template <typename TNum>
static inline constexpr TNum cube(TNum num)
{
  return num * num * num;
}

static constexpr const real_t PI = static_cast<real_t>(3.14159265358979323846);