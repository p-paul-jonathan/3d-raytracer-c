#include "./camera.h"

bool in_camera_range(Camera camera, float ray_parameter) {
  return camera.min_render_distance <= ray_parameter &&
         camera.max_render_distance >= ray_parameter;
}
