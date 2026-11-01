#ifndef COLORING_H
#define COLORING_H

#include <Origin/function/common.h>

// Soft highlight compression: colors up to 0.8 are untouched, brighter values roll off smoothly.
vec3 tonemap(vec3 color) {
  vec3 over = max(color - vec3_splat(0.8), vec3_splat(0.0));
  return color - over + over / (vec3_splat(1.0) + 4.0 * over);
}

#endif
