#ifndef WEATHER_H
#define WEATHER_H

#include <Origin/function/common.h>

// Rain and snow particles take the light of the place they fall in.
vec3 weatherLight(vec3 color, vec3 light) {
  return color * light;
}

#endif
