#ifndef CAMERA_H
#define CAMERA_H

#include "vector_3d.h"

typedef struct {
  float width;
  float height;

  float viewport_width;
  float viewport_height;
  float viewport_distance;

  Vector3D position;

  float min_render_distance;
  float max_render_distance;
} Camera;

bool in_camera_range(Camera camera, float ray_parameter);

#endif /* CAMERA_H */
