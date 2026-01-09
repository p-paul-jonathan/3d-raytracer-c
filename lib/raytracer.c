#include "./raytracer.h"
#include "camera.h"
#include "scene.h"
#include "sphere.h"
#include "vector_3d.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  float closest_t;
  Sphere *closest_sphere;
  bool hit_sphere;
} Intersection;

static inline void put_pixel(int x, int y, uint32_t color, Camera *camera,
                             uint32_t *framebuffer) {
  int screen_x = (camera->width / 2) + x;
  int screen_y = (camera->height / 2) - y;

  if (screen_x < 0 || screen_x >= camera->width || screen_y < 0 ||
      screen_y >= camera->height) {
    return;
  }

  framebuffer[screen_y * (int)camera->width + screen_x] = color;
}

static inline Vector3D canvas_to_viewport(int x, int y, Camera *camera) {
  return (Vector3D){x * (camera->viewport_width / camera->width),
                    y * (camera->viewport_height / camera->height),
                    camera->viewport_distance};
}

static inline Intersection closest_intersection(Camera *camera, Scene *scene,
                                                Vector3D viewport_vector) {
  Intersection result = {.closest_t = camera->max_render_distance,
                         .closest_sphere = NULL,
                         .hit_sphere = false};

  for (int i = 0; i < scene->spheres_count; i++) {
    SphereIntersections sphere_intersections = calculate_sphere_intersection(
        camera, &scene->spheres[i], viewport_vector);

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

static inline uint32_t trace_ray(Camera *camera, Scene *scene,
                                 Vector3D viewport_vector) {
  Intersection intersection =
      closest_intersection(camera, scene, viewport_vector);

  if (!intersection.hit_sphere) {
    return scene->default_background_color;
  }

  return intersection.closest_sphere->color;
}

void main_raytracer(Scene *scene, Camera *camera, uint32_t *framebuffer) {
  int half_width = camera->width / 2;
  int half_height = camera->height / 2;

  for (int x = -half_width; x < half_width; x++) {
    for (int y = -half_height; y < half_height; y++) {
      Vector3D viewport_vector = canvas_to_viewport(x, y, camera);
      uint32_t color = trace_ray(camera, scene, viewport_vector);
      put_pixel(x, y, color, camera, framebuffer);
    }
  }
}
