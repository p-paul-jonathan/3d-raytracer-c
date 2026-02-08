#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>

#include "light.h"
#include "sphere.h"
#include "vector_color.h"

typedef struct {
  Sphere *spheres;
  int spheres_count;
  Light *lights;
  int lights_count;
  VectorColor default_background_color;
} Scene;

#endif /* SCENE_H */
