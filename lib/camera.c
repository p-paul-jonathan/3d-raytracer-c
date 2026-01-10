#include <math.h>

#include "./camera.h"
#include "./constants.h"

bool in_camera_range(Camera camera, float ray_parameter) {
  return camera.ray_t_min <= ray_parameter && camera.ray_t_max >= ray_parameter;
}

void camera_move_front(Camera *camera, float amount) {
  camera->position = vector_3d_add(
      camera->position, vector_3d_multiply_scalar(camera->forward, amount));
}

void camera_move_back(Camera *camera, float amount) {
  camera->position = vector_3d_subtract(
      camera->position, vector_3d_multiply_scalar(camera->forward, amount));
}

void camera_move_right(Camera *camera, float amount) {
  camera->position = vector_3d_add(
      camera->position, vector_3d_multiply_scalar(camera->right, amount));
}

void camera_move_left(Camera *camera, float amount) {
  camera->position = vector_3d_subtract(
      camera->position, vector_3d_multiply_scalar(camera->right, amount));
}

void camera_move_up(Camera *camera, float amount) {
  camera->position = vector_3d_add(
      camera->position, vector_3d_multiply_scalar(camera->up, amount));
}

void camera_move_down(Camera *camera, float amount) {
  camera->position = vector_3d_subtract(
      camera->position, vector_3d_multiply_scalar(camera->up, amount));
}

void camera_pitch_up(Camera *camera, float rotate) {
  camera->pitch += rotate;

  if (camera->pitch > camera->pitch_range) {
    camera->pitch = camera->pitch_range;
  }
}

void camera_pitch_down(Camera *camera, float rotate) {
  camera->pitch -= rotate;

  if (camera->pitch < -camera->pitch_range) {
    camera->pitch = -camera->pitch_range;
  }
}

void camera_yaw_right(Camera *camera, float rotate) {
  camera->yaw -= rotate;

  if (camera->yaw < -(2 * MATH_PI)) {
    camera->yaw = 0;
  }
}

void camera_yaw_left(Camera *camera, float rotate) {
  camera->yaw += rotate;

  if (camera->yaw > 2 * MATH_PI) {
    camera->yaw = 0;
  }
}

void camera_roll_left(Camera *camera, float rotate) {
  camera->roll -= rotate;

  if (camera->roll > camera->roll_range) {
    camera->roll = camera->roll_range;
  }
}

void camera_roll_right(Camera *camera, float rotate) {
  camera->roll += rotate;

  if (camera->roll > camera->roll_range) {
    camera->roll = camera->roll_range;
  }
}

void camera_update_orientation(Camera *camera) {
  const Vector3D world_up = vector_3d_init(0.0f, 1.0f, 0.0f);

  Vector3D forward = vector_3d_init(cosf(camera->pitch) * sinf(camera->yaw),
                                    sinf(camera->pitch),
                                    cosf(camera->pitch) * cosf(camera->yaw));
  forward = vector_3d_normalize(forward);

  Vector3D right = vector_3d_cross_product(forward, world_up);
  right = vector_3d_normalize(right);

  Vector3D up = vector_3d_cross_product(right, forward);
  up = vector_3d_normalize(up);

  if (camera->roll != 0.0f) {
    float cos_r = cosf(camera->roll);
    float sin_r = sinf(camera->roll);

    Vector3D rolled_right =
        vector_3d_add(vector_3d_multiply_scalar(right, cos_r),
                      vector_3d_multiply_scalar(up, -sin_r));

    Vector3D rolled_up = vector_3d_add(vector_3d_multiply_scalar(right, sin_r),
                                       vector_3d_multiply_scalar(up, cos_r));

    right = rolled_right;
    up = rolled_up;
  }

  camera->forward = forward;
  camera->right = right;
  camera->up = up;
}
