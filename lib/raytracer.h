#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <stdint.h>

#include "camera.h"
#include "scene.h"

void main_raytracer(Scene *scene, Camera *camera, uint32_t *framebuffer, bool low_resolution);

#endif /* RAYTRACER_H */
