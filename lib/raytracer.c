#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "raytracer.h"
#include "camera.h"
#include "scene.h"
#include "sphere.h"
#include "vector_3d.h"
#include "vector_color.h"

typedef struct {
  float closest_t;
  Sphere *closest_sphere;
  bool hit_sphere;
} Intersection;

static inline void put_pixel(int x, int y, VectorColor color, Camera *camera,
                             uint32_t *framebuffer) {
  int screen_x = (camera->width / 2) + x;
  int screen_y = (camera->height / 2) - y;

  if (screen_x < 0 || screen_x >= camera->width || screen_y < 0 ||
      screen_y >= camera->height) {
    return;
  }

  framebuffer[screen_y * (int)camera->width + screen_x] = vector_color_to_rgb_color(color);
}

static inline Vector3D canvas_to_viewport(int x, int y, Camera *camera) {
  return vector_3d_init(x * (camera->viewport_width / camera->width),
                        y * (camera->viewport_height / camera->height),
                        camera->viewport_distance);
}

static inline Intersection closest_intersection(Camera *camera, Scene *scene,
                                                Vector3D ray_direction) {
  Intersection result = {.closest_t = camera->ray_t_max,
                         .closest_sphere = NULL,
                         .hit_sphere = false};

  for (int i = 0; i < scene->spheres_count; i++) {
    SphereIntersections sphere_intersections = calculate_sphere_intersection(
        camera, &scene->spheres[i], ray_direction);

    if (in_camera_range(*camera, sphere_intersections.t1) &&
        sphere_intersections.t1 < result.closest_t) {
      result.closest_t = sphere_intersections.t1;
      result.closest_sphere = &scene->spheres[i];
      result.hit_sphere = true;
    }

    if (in_camera_range(*camera, sphere_intersections.t2) &&
        sphere_intersections.t2 < result.closest_t) {
      result.closest_t = sphere_intersections.t2;
      result.closest_sphere = &scene->spheres[i];
      result.hit_sphere = true;
    }
  }

  return result;
}

static inline VectorColor trace_ray(Camera *camera, Scene *scene,
                                 Vector3D ray_direction) {
  Intersection intersection =
      closest_intersection(camera, scene, ray_direction);

  if (!intersection.hit_sphere) {
    return scene->default_background_color;
  }

  return intersection.closest_sphere->color;
}

void main_raytracer(Scene *scene, Camera *camera, uint32_t *framebuffer,
                    bool low_resolution) {
  int half_width = camera->width / 2;
  int half_height = camera->height / 2;

  int iterator = low_resolution ? 8 : 1;

  for (int x = -half_width; x < half_width; x += iterator) {
    for (int y = -half_height; y < half_height; y += iterator) {

      Vector3D viewport = canvas_to_viewport(x, y, camera);

      Vector3D ray_direction = vector_3d_add(
          vector_3d_add(vector_3d_multiply_scalar(camera->forward, viewport.z),
                        vector_3d_multiply_scalar(camera->right, viewport.x)),
          vector_3d_multiply_scalar(camera->up, viewport.y));

      VectorColor color = trace_ray(camera, scene, ray_direction);
      if (low_resolution) {
        for (int dx = 0; dx < iterator; dx++) {
          for (int dy = 0; dy < iterator; dy++) {
            put_pixel(x + dx, y + dy, color, camera, framebuffer);
          }
        }
      } else {
        put_pixel(x, y, color, camera, framebuffer);
      }
    }
  }
}
