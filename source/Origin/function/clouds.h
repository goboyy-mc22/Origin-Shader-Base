#ifndef CLOUDS_H
#define CLOUDS_H

#include <Origin/function/common.h>
#include <Origin/function/coloring.h>

vec4 cloudColor(vec4 color) {
  return vec4(tonemap(color.rgb), color.a);
}

#endif
