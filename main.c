#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib/camera.h"
#include "lib/light.h"
#include "lib/raytracer.h"
#include "lib/scene.h"
#include "lib/sphere.h"
#include "lib/vector_3d.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

#define VIEWPORT_WIDTH 1.6
#define VIEWPORT_HEIGHT 0.9
#define VIEWPORT_DISTANCE 1

#define MAX_RENDER_DISTANCE INFINITY
#define MIN_RENDER_DISTANCE 1

#define DEFAULT_BACKGROUND_COLOR 0xFF000000

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static uint32_t *framebuffer = NULL;

static Camera *camera = NULL;
static Scene *scene = NULL;

static inline void clear_frame_buffer(uint32_t color) {
  size_t pixel_count = (size_t)WINDOW_WIDTH * WINDOW_HEIGHT;
  for (size_t i = 0; i < pixel_count; i++) {
    framebuffer[i] = color;
  }
}

static inline void initialize_camera_and_scene(void) {
  camera = malloc(sizeof(Camera));
  scene = malloc(sizeof(Scene));

  if (!camera || !scene) {
    SDL_Log("Out of memory");
    exit(1);
  }

  camera->position = (Vector3D){0, 0, -2};

  camera->width = WINDOW_WIDTH;
  camera->height = WINDOW_HEIGHT;

  camera->viewport_width = VIEWPORT_WIDTH;
  camera->viewport_height = VIEWPORT_HEIGHT;
  camera->viewport_distance = VIEWPORT_DISTANCE;

  camera->min_render_distance = MIN_RENDER_DISTANCE;
  camera->max_render_distance = MAX_RENDER_DISTANCE;

  scene->spheres_count = 4;
  scene->spheres = malloc(sizeof(Sphere) * scene->spheres_count);
  if (!scene->spheres) {
    SDL_Log("Out of memory");
    exit(1);
  }
  scene->spheres[0] = (Sphere){(Vector3D){0, -1, 3}, 1, 0xFFFF0000};
  scene->spheres[1] = (Sphere){(Vector3D){2, 0, 4}, 1, 0xFF0000FF};
  scene->spheres[2] = (Sphere){(Vector3D){-2, 0, 4}, 1, 0xFF00FF00};
  scene->spheres[3] = (Sphere){(Vector3D){0, -5001, 0}, 5000, 0xFFFFFF00};

  scene->lights_count = 1;
  scene->lights = malloc(sizeof(Light) * scene->lights_count);
  if (!scene->lights) {
    SDL_Log("Out of memory");
    exit(1);
  }
  scene->lights[0] = (Light){0.2, AMBIENT, (Vector3D){0, 0, 0}};

  scene->default_background_color = DEFAULT_BACKGROUND_COLOR;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("3D Raytracing", "1.0", "com.example.raytracer");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Framebuffer Renderer", WINDOW_WIDTH,
                                   WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window,
                                   &renderer)) {
    SDL_Log("Window/Renderer creation failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH,
                              WINDOW_HEIGHT);

  if (!texture) {
    SDL_Log("Texture creation failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  framebuffer = malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
  if (!framebuffer) {
    SDL_Log("Framebuffer allocation failed");
    return SDL_APP_FAILURE;
  }

  initialize_camera_and_scene();

  clear_frame_buffer(scene->default_background_color);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  clear_frame_buffer(scene->default_background_color);

  main_raytracer(scene, camera, framebuffer);

  SDL_UpdateTexture(texture, NULL, framebuffer,
                    WINDOW_WIDTH * sizeof(uint32_t));

  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (framebuffer) {
    free(framebuffer);
  }

  if (texture) {
    SDL_DestroyTexture(texture);
  }

  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }

  if (window) {
    SDL_DestroyWindow(window);
  }

  if (scene) {
    free(scene->spheres);
    free(scene->lights);
    free(scene);
  }

  if (camera) {
    free(camera);
  }

  SDL_Quit();
}
