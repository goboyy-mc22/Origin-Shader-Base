#ifndef WATER_H
#define WATER_H

#include <Origin/function/common.h>

// Schlick fresnel: how much of the sky is mirrored at this viewing angle.
float fresnel(float cosTheta, float f0) {
  float x = 1.0 - saturate(cosTheta);
  float x2 = x * x;
  return f0 + (1.0 - f0) * x2 * x2 * x;
}

// Water surface: mirrors the sky at grazing angles and becomes more opaque there.
vec4 shadeWater(vec4 color, vec3 viewDir, vec3 skyColor) {
  float f = fresnel(abs(viewDir.y), 0.04);
  color.rgb = mix(color.rgb, skyColor, f * 0.8);
  color.a = mix(color.a, 1.0, f);
  return color;
}

#endif
