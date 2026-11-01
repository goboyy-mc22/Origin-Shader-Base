#ifndef COMMON_H
#define COMMON_H

#include <bgfx_shader.sh>
#include <Origin/configuration.h>

#define PI 3.14159265

float luminance(vec3 color) {
  return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

mat2 rotation2d(float angle) {
  float s = sin(angle);
  float c = cos(angle);
  return mtxFromRows(vec2(c, -s), vec2(s, c));
}

#endif
