#ifndef LIGHT_H
#define LIGHT_H

#include "vector_3d.h"

enum lighting_type { AMBIENT, POINT, DIRECTIONAL };

typedef struct {
  float intensity;
  enum lighting_type type;
  Vector3D position;
} Light;

#endif /* CAMERA_H */
