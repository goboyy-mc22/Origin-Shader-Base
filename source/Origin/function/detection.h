#ifndef DETECTION_H
#define DETECTION_H

#include <bgfx_shader.sh>

// Time of day from the fog brightness: 0.0 night, 1.0 day.
float detectDay(vec3 fogColor) {
  return saturate(dot(fogColor, vec3(0.5, 0.7, 0.5)));
}

// Sunrise and sunset have a red-tinted fog: 0.0 none, 1.0 full.
float detectDawn(vec3 fogColor) {
  return saturate(3.0 * (fogColor.r - fogColor.b));
}

// Rain pulls the fog range in. fogControl = (fog start, fog end, render distance in blocks).
// Returns 0.0 for clear weather and 1.0 for full rain.
float detectRain(vec3 fogControl) {
  vec2 clear = vec2(0.5 + 20.0 / fogControl.z, 1.0);
  vec2 rain = vec2(0.23, 0.70);
  vec2 amount = clamp((fogControl.xy - clear) / (rain - clear), vec2(0.0, 0.0), vec2(1.0, 1.0));
  float value = amount.x * amount.y;
  return value * value * (3.0 - 2.0 * value);
}

// Underwater: the fog starts at the camera, ends close by and is blue or green.
bool detectUnderwater(vec3 fogColor, vec2 fogControl) {
  return fogControl.x == 0.0 && fogControl.y < 0.8 && (fogColor.b > fogColor.r || fogColor.g > fogColor.r);
}

#endif
