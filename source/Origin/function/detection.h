#ifndef DETECTION_H
#define DETECTION_H

#include <Origin/function/common.h>

// What the shader knows about the world, detected from vanilla uniforms.
struct Environment {
  bool end;
  bool nether;
  bool underwater;
  float rain;      // 0.0 clear, 1.0 full rain
  float day;       // 0.0 night, 1.0 full day
  float dawn;      // 0.0 none, 1.0 sunrise or sunset
  vec3 fogColor;
};

// The End has a purple fog color (set by the game).
bool detectEnd(vec3 fogColor) {
  return fogColor.r == fogColor.b && (fogColor.r - fogColor.g > 0.24 || (fogColor.g == 0.0 && fogColor.r > 0.1));
}

// The Nether fog distance depends on the render distance; estimate what a Nether value would be.
// Lava is counted as Nether.
bool detectNether(vec3 fogColor, vec2 fogControl) {
  float expectedStart = 0.029 + 0.09 * fogControl.y * fogControl.y;
  bool netherFog = fogControl.x < 0.14 && abs(fogControl.x - expectedStart) < 0.02 && (fogColor.r + fogColor.g) > 0.0;
  bool inLava = fogControl.x == 0.0 && fogColor.b == 0.0 && fogColor.g < 0.18 && fogColor.r - fogColor.g > 0.1;
  return netherFog || inLava;
}

// Underwater: fog starts at the camera and ends close by, with a blue or green color.
bool detectUnderwater(vec3 fogColor, vec2 fogControl) {
  return fogControl.x == 0.0 && fogControl.y < 0.8 && (fogColor.b > fogColor.r || fogColor.g > fogColor.r);
}

// Rain pulls the fog range in. Compare it with the clear-weather range (which depends on render distance).
// fogControl = (start, end, render distance in blocks)
float detectRain(vec3 fogControl) {
  vec2 clear = vec2(0.5 + 20.0 / fogControl.z, 1.0);
  vec2 rain = vec2(0.23, 0.70);
  vec2 amount = clamp((fogControl.xy - clear) / (rain - clear), vec2(0.0, 0.0), vec2(1.0, 1.0));
  float value = amount.x * amount.y;
  return value * value * (3.0 - 2.0 * value);
}

// Time of day from the fog brightness: 0.0 night to 1.0 day.
float detectDay(vec3 fogColor) {
  return saturate(dot(fogColor, vec3(0.5, 0.7, 0.5)));
}

// Sunrise and sunset are red-tinted fog.
float detectDawn(vec3 fogColor) {
  return saturate(3.0 * (fogColor.r - fogColor.b));
}

Environment detectEnvironment(vec3 fogColor, vec3 fogControl) {
  Environment env;
  env.end = detectEnd(fogColor);
  env.nether = detectNether(fogColor, fogControl.xy);
  env.underwater = detectUnderwater(fogColor, fogControl.xy);
  env.rain = detectRain(fogControl);
  env.day = detectDay(fogColor);
  env.dawn = detectDawn(fogColor);
  env.fogColor = fogColor;
  return env;
}

// Sun and moon directions from the time of day (0.0 noon, 0.25 sunset, 0.5 midnight, 0.75 sunrise).
vec3 sunDirection(float timeOfDay) {
  float angle = 2.0 * PI * timeOfDay;
  return vec3(sin(angle), cos(angle), 0.0);
}

vec3 moonDirection(float timeOfDay) {
  return -sunDirection(timeOfDay);
}

// ---- Geometry detection (terrain vertex shader) ----------------------------

// Water vertices are the translucent ones.
bool isWater(vec4 vertexColor) {
  return vertexColor.a < 0.95;
}

// Biome-tinted vertices (grass, leaves, vines) have a non-gray color.
bool isTinted(vec4 vertexColor) {
  return vertexColor.r != vertexColor.g || vertexColor.r != vertexColor.b;
}

// Leaves are drawn with corners exactly on block corners.
bool isBlockCorner(vec3 chunkPosition) {
  vec3 corner = abs(fract(chunkPosition + 0.5) - 0.5);
  return corner.x < 0.001 && corner.y < 0.001 && corner.z < 0.001;
}

#endif
