#include <math.h>
#include <stdint.h>

#include "vector_color.h"

VectorColor vector_color_init(float red, float green, float blue) {
  return (VectorColor){red, green, blue};
}

VectorColor vector_color_red(void) {
  return vector_color_init(1.0f, 0.0f, 0.0f);
}
VectorColor vector_color_green(void) {
  return vector_color_init(0.0f, 1.0f, 0.0f);
}
VectorColor vector_color_blue(void) {
  return vector_color_init(0.0f, 0.0f, 1.0f);
}
VectorColor vector_color_cyan(void) {
  return vector_color_init(0.0f, 1.0f, 1.0f);
}
VectorColor vector_color_magenta(void) {
  return vector_color_init(1.0f, 0.0f, 1.0f);
}
VectorColor vector_color_yellow(void) {
  return vector_color_init(1.0f, 1.0f, 0.0f);
}
VectorColor vector_color_black(void) {
  return vector_color_init(0.0f, 0.0f, 0.0f);
}
VectorColor vector_color_white(void) {
  return vector_color_init(1.0f, 1.0f, 1.0f);
}

bool vector_color_equal(VectorColor a, VectorColor b, float epsilon) {
  if (epsilon <= 0.0f) {
    return a.red == b.red && a.green == b.green && a.blue == b.blue;
  }

  return fabsf(a.red - b.red) <= epsilon &&
         fabsf(a.green - b.green) <= epsilon &&
         fabsf(a.blue - b.blue) <= epsilon;
}

VectorColor vector_color_clamp(VectorColor v) {
  const float min = 0.0f;
  const float max = 1.0f;

  return vector_color_init(fminf(max, fmaxf(min, v.red)),
                           fminf(max, fmaxf(min, v.green)),
                           fminf(max, fmaxf(min, v.blue)));
}

VectorColor vector_color_multiply_scalar(VectorColor a, float k) {
  return vector_color_init(a.red * k, a.green * k, a.blue * k);
}

uint32_t vector_color_to_rgb_color(VectorColor v) {
  v = vector_color_clamp(v);

  uint8_t r = (uint8_t)(v.red * 255.0f);
  uint8_t g = (uint8_t)(v.green * 255.0f);
  uint8_t b = (uint8_t)(v.blue * 255.0f);

  /* ARGB: 0xAARRGGBB (opaque alpha) */
  return (0xFFu << 24) | (r << 16) | (g << 8) | b;
}
