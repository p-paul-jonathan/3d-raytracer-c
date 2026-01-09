#include "sphere.h"
#include "camera.h"
#include "vector_3d.h"
#include <math.h>

SphereIntersections calculate_sphere_intersection(Camera *camera,
                                                  Sphere *sphere,
                                                  Vector3D viewport_vector) {
  Vector3D camera_to_center_vector =
      vector_3d_subtract(camera->position, sphere->center);

  float a = vector_3d_dot_product(viewport_vector, viewport_vector);
  float b = vector_3d_dot_product(camera_to_center_vector, viewport_vector) * 2;
  float c =
      vector_3d_dot_product(camera_to_center_vector, camera_to_center_vector) -
      (sphere->radius * sphere->radius);

  float discriminant = (b * b) - (4 * a * c);

  if (discriminant < 0) {
    return (SphereIntersections){camera->max_render_distance,
                                 camera->max_render_distance};
  }

  float t1 = (-b + sqrtf(discriminant)) / 2 * a;
  float t2 = (-b - sqrtf(discriminant)) / 2 * a;

  return (SphereIntersections){t1, t2};
}
