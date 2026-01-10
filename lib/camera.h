#ifndef CAMERA_H
#define CAMERA_H

#include "./vector_3d.h"

typedef struct {
  float width;
  float height;

  float viewport_width;
  float viewport_height;
  float viewport_distance;

  Vector3D position;

  float ray_t_min;
  float ray_t_max;

  float pitch;
  float yaw;
  float roll;

  float pitch_range;
  float roll_range;

  float move_speed;
  float rotate_speed;

  Vector3D forward;
  Vector3D right;
  Vector3D up;
} Camera;

bool in_camera_range(Camera camera, float ray_parameter);

void camera_move_up(Camera *camera, float move);
void camera_move_down(Camera *camera, float move);
void camera_move_left(Camera *camera, float move);
void camera_move_right(Camera *camera, float move);
void camera_move_front(Camera *camera, float move);
void camera_move_back(Camera *camera, float move);

void camera_pitch_up(Camera *camera, float rotate);
void camera_pitch_down(Camera *camera, float rotate);
void camera_yaw_left(Camera *camera, float rotate);
void camera_yaw_right(Camera *camera, float rotate);
void camera_roll_left(Camera *camera, float rotate);
void camera_roll_right(Camera *camera, float rotate);

void camera_update_orientation(Camera *camera);
#endif /* CAMERA_H */
