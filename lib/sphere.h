#ifndef SPHERE_H
#define SPHERE_H

#include <stdint.h>

#include "camera.h"
#include "vector_3d.h"
#include "vector_color.h"

typedef struct {
  Vector3D center;
  float radius;
  VectorColor color;
  bool is_light_source;
} Sphere;

typedef struct {
  float t1;
  float t2;
} SphereIntersections;

SphereIntersections calculate_sphere_intersection(Camera *camera,
                                                  Sphere *sphere,
                                                  Vector3D ray_direction);

#endif /* SPHERE_H */
