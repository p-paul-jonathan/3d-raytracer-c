#include <math.h>

#include "./vector_3d.h"

Vector3D vector_3d_init(float x, float y, float z) {
  return (Vector3D){x, y, z};
}

Vector3D vector_3d_zero(void) { return vector_3d_init(0.0f, 0.0f, 0.0f); }

Vector3D vector_3d_add(const Vector3D a, const Vector3D b) {
  return vector_3d_init(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3D vector_3d_subtract(const Vector3D a, const Vector3D b) {
  return vector_3d_init(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3D vector_3d_negate(const Vector3D v) {
  return vector_3d_init(-v.x, -v.y, -v.z);
}

float vector_3d_dot_product(const Vector3D a, const Vector3D b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3D vector_3d_cross_product(const Vector3D a, const Vector3D b) {
  return vector_3d_init(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x);
}

bool vector_3d_equal(const Vector3D a, const Vector3D b, float epsilon) {
  if (epsilon == 0.0f) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
  }

  return fabsf(a.x - b.x) <= epsilon && fabsf(a.y - b.y) <= epsilon &&
         fabsf(a.z - b.z) <= epsilon;
}

float vector_3d_magnitude(const Vector3D v) {
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3D vector_3d_multiply_scalar(const Vector3D v, float k) {
  return vector_3d_init(v.x * k, v.y * k, v.z * k);
}

Vector3D vector_3d_normalize(const Vector3D v) {
  float mag = vector_3d_magnitude(v);

  if (mag == 0.0f) {
    return vector_3d_zero();
  }

  return vector_3d_multiply_scalar(v, 1.0f / mag);
}

Vector3D vector_3d_reflect(const Vector3D v, const Vector3D normal) {
  float dot = vector_3d_dot_product(v, normal);
  return vector_3d_subtract(v, vector_3d_multiply_scalar(normal, 2.0f * dot));
}
