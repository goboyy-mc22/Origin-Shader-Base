#ifndef WAVE_H
#define WAVE_H

#include <Origin/function/common.h>

// Waves fade out with distance (1.0 near, 0.0 beyond WAVE_RANGE).
float waveFade(float camDist) {
  return 1.0 - smoothstep(0.5, 1.0, saturate(camDist / WAVE_RANGE));
}

// Gusty wind in [0, ~1.4]; rain makes it stronger.
float windStrength(float t, float rain) {
  float gust = 0.5 + 0.3 * sin(t * 0.45) + 0.2 * sin(t * 1.1 + 1.7);
  return gust + 0.4 * rain;
}

// Plant and leaf sway: offset to add to the vertex position.
// skyLight keeps plants in caves and under roofs still.
vec3 plantSway(vec3 worldPos, float t, float wind, float skyLight) {
  float phase = dot(worldPos, vec3(0.6, 0.3, 0.45));
  float swing = 0.6 * sin(t * 1.8 + phase) + 0.4 * sin(t * 2.7 + phase * 1.3);
  float amount = PLANT_SWAY_STRENGTH * wind * skyLight * swing;
  return vec3(amount, -0.5 * abs(amount), 0.6 * amount);
}

// Texture row position in the terrain atlas (0.0 top of a tile, 1.0 bottom).
// Assumes the vanilla atlas of 32 tile rows.
float tileRowPosition(vec2 uv) {
  return fract(uv.y * 32.0);
}

// Water surface height offset in blocks.
float waterWave(vec3 worldPos, float t) {
  float a = sin(dot(worldPos.xz, vec2(1.2, 0.7)) + t * 1.4);
  float b = sin(dot(worldPos.xz, vec2(-0.8, 1.5)) + t * 1.9);
  return WATER_WAVE_HEIGHT * 0.5 * (a + b);
}

#endif
