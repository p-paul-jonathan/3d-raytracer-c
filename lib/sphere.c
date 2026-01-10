#include "sphere.h"
#include "camera.h"
#include "vector_3d.h"
#include <math.h>

SphereIntersections calculate_sphere_intersection(Camera *camera,
                                                  Sphere *sphere,
                                                  Vector3D ray_direction) {
  Vector3D origin_to_center =
      vector_3d_subtract(camera->position, sphere->center);

  float quadratic_a = vector_3d_dot_product(ray_direction, ray_direction);
  float quadratic_b =
      vector_3d_dot_product(origin_to_center, ray_direction) * 2;
  float quadratic_c =
      vector_3d_dot_product(origin_to_center, origin_to_center) -
      (sphere->radius * sphere->radius);

  float discriminant =
      (quadratic_b * quadratic_b) - (4 * quadratic_a * quadratic_c);

  if (discriminant < 0) {
    return (SphereIntersections){camera->ray_t_max,
                                 camera->ray_t_max};
  }

  float t1 = (-quadratic_b + sqrtf(discriminant)) / 2 * quadratic_a;
  float t2 = (-quadratic_b - sqrtf(discriminant)) / 2 * quadratic_a;

  return (SphereIntersections){t1, t2};
}
