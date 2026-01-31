#ifndef VECTOR_COLOR_H
#define VECTOR_COLOR_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  float red;
  float green;
  float blue;
} VectorColor;

VectorColor vector_color_init(float red, float green, float blue);

VectorColor vector_color_red(void);
VectorColor vector_color_green(void);
VectorColor vector_color_blue(void);
VectorColor vector_color_cyan(void);
VectorColor vector_color_magenta(void);
VectorColor vector_color_yellow(void);
VectorColor vector_color_black(void);
VectorColor vector_color_white(void);

bool vector_color_equal(VectorColor a, VectorColor b, float epsilon);

VectorColor vector_color_multiply_scalar(VectorColor a, float k);

VectorColor vector_color_clamp(VectorColor v);

uint32_t vector_color_to_rgb_color(VectorColor v);

#endif // VECTOR_COLOR_H
