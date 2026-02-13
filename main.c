#include "lib/vector_color.h"
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_scancode.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib/camera.h"
#include "lib/constants.h"
#include "lib/light.h"
#include "lib/raytracer.h"
#include "lib/scene.h"
#include "lib/sphere.h"
#include "lib/vector_3d.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static uint32_t *framebuffer = NULL;

static Camera *camera = NULL;
static Scene *scene = NULL;

static uint64_t last_ticks = 0;

static int hd_rendered = false;
static int show_hd = true;

static inline void clear_framebuffer(VectorColor color) {
  size_t count = (size_t)WINDOW_WIDTH * WINDOW_HEIGHT;
  for (size_t i = 0; i < count; i++) {
    framebuffer[i] = vector_color_to_rgb_color(color);
  }
}

static void initialize_camera(void) {
  camera = malloc(sizeof(Camera));
  if (!camera) {
    SDL_Log("Out of memory (Camera)");
    exit(1);
  }

  camera->position = vector_3d_init(0.0f, 0.0f, -3.0f);

  camera->width = WINDOW_WIDTH;
  camera->height = WINDOW_HEIGHT;

  camera->viewport_width = VIEWPORT_WIDTH;
  camera->viewport_height = VIEWPORT_HEIGHT;
  camera->viewport_distance = VIEWPORT_DISTANCE;

  camera->ray_t_min = RAY_T_MIN;
  camera->ray_t_max = RAY_T_MAX;

  camera->yaw = 0.0f;
  camera->pitch = 0.0f;
  camera->roll = 0.0f;

  camera->pitch_range = MATH_PI / 3.0f; /* ±60° */
  camera->roll_range = MATH_PI / 4.0f;  /* ±45° */

  camera->move_speed = CAMERA_MOVE_SPEED;
  camera->rotate_speed = CAMERA_ROTATE_SPEED;

  camera_update_orientation(camera);
}

static void initialize_scene(void) {
  scene = malloc(sizeof(Scene));
  if (!scene) {
    SDL_Log("Out of memory (Scene)");
    exit(1);
  }

  scene->spheres_count = 5;
  scene->spheres = malloc(sizeof(Sphere) * scene->spheres_count);

  if (!scene->spheres) {
    SDL_Log("Out of memory (Sphere)");
    exit(1);
  }

  scene->spheres[0] =
      (Sphere){vector_3d_init(0, -1, 3), 1, vector_color_red(), false, 500};
  scene->spheres[1] =
      (Sphere){vector_3d_init(2, 0, 4), 1, vector_color_blue(), false, 500};
  scene->spheres[2] =
      (Sphere){vector_3d_init(2, 1, 0), 0.05, vector_color_white(), true, -1};
  scene->spheres[3] =
      (Sphere){vector_3d_init(-2, 0, 4), 1, vector_color_green(), false, 500};
  scene->spheres[4] = (Sphere){vector_3d_init(0, -5001, 0), 5000,
                               vector_color_yellow(), false, 1000};

  scene->lights_count = 3;
  scene->lights = malloc(sizeof(Light) * scene->lights_count);
  if (!scene->lights) {
    SDL_Log("Out of memory (Light)");
    exit(1);
  }

  scene->lights[0] = (Light){0.2f, AMBIENT, vector_3d_init(0, 0, 0)};
  scene->lights[1] = (Light){0.6f, POINT, vector_3d_init(2, 1, 0)};
  scene->lights[2] = (Light){0.2f, DIRECTIONAL, vector_3d_init(1, 4, 4)};

  scene->default_background_color = vector_color_black();
}

static void handle_camera_input(Camera *camera, const bool *keys, float move,
                                float rotate) {
  if (keys[SDL_SCANCODE_W]) {
    camera_move_front(camera, move);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_S]) {
    camera_move_back(camera, move);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_A]) {
    camera_move_left(camera, move);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_D]) {
    camera_move_right(camera, move);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_K]) {
    camera_move_up(camera, move);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_J]) {
    camera_move_down(camera, move);
    show_hd = false;
    hd_rendered = false;
  }

  if (keys[SDL_SCANCODE_UP]) {
    camera_pitch_up(camera, rotate);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_DOWN]) {
    camera_pitch_down(camera, rotate);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_LEFT]) {
    camera_yaw_left(camera, rotate);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_RIGHT]) {
    camera_yaw_right(camera, rotate);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_Q]) {
    camera_roll_left(camera, rotate);
    show_hd = false;
    hd_rendered = false;
  }
  if (keys[SDL_SCANCODE_E]) {
    camera_roll_right(camera, rotate);
    show_hd = false;
    hd_rendered = false;
  }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("Raytracer", "1.0", "com.example.raytracer");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Raytracer", WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Window creation failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH,
                              WINDOW_HEIGHT);

  framebuffer = malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
  if (!framebuffer) {
    SDL_Log("Framebuffer allocation failed");
    return SDL_APP_FAILURE;
  }

  initialize_camera();
  initialize_scene();

  clear_framebuffer(scene->default_background_color);

  last_ticks = SDL_GetTicks();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  uint64_t now = SDL_GetTicks();
  float delta_time = (now - last_ticks) / 1000.0f;
  last_ticks = now;

  const bool *keys = SDL_GetKeyboardState(NULL);

  bool moving = keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_S] ||
                keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_D] ||
                keys[SDL_SCANCODE_K] || keys[SDL_SCANCODE_J] ||
                keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN] ||
                keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT] ||
                keys[SDL_SCANCODE_Q] || keys[SDL_SCANCODE_E];

  if (!moving) {
    show_hd = true;
  }

  handle_camera_input(camera, keys, camera->move_speed * delta_time,
                      camera->rotate_speed * delta_time);

  camera_update_orientation(camera);

  if (!show_hd || !hd_rendered) {

    clear_framebuffer(scene->default_background_color);

    main_raytracer(scene, camera, framebuffer, !show_hd);

    if (show_hd) {
      hd_rendered = true;
    }
  }

  SDL_UpdateTexture(texture, NULL, framebuffer,
                    WINDOW_WIDTH * sizeof(uint32_t));

  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  free(framebuffer);

  if (scene) {
    free(scene->spheres);
    free(scene->lights);
    free(scene);
  }

  free(camera);

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
