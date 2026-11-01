#ifndef LIGHTING_H
#define LIGHTING_H

#include <Origin/function/common.h>

// Apply the light map sample (block light and sky light) to a surface color.
vec3 shadeWithLightmap(vec3 color, vec3 lightmap) {
  return color * lightmap;
}

// Blend an entity overlay (hurt flash, creeper fuse) over a color. overlay.a is the amount.
vec3 blendOverlay(vec3 color, vec4 overlay) {
  return mix(color, overlay.rgb, overlay.a);
}

#endif
