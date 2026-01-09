#ifndef SPHERE_H
#define SPHERE_H

#include "camera.h"
#include "vector_3d.h"
#include <stdint.h>

typedef struct {
  Vector3D center;
  float radius;
  uint32_t color;
} Sphere;

typedef struct {
  float t1;
  float t2;
} SphereIntersections;

SphereIntersections calculate_sphere_intersection(Camera *camera,
                                                  Sphere *sphere,
                                                  Vector3D viewport_vector);

#endif /* SPHERE_H */
