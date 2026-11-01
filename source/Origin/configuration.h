#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// Origin Shader Base - configuration
// Only the important values live here: sky colors, fog range and a few effect strengths.
// Colors are linear RGB.

// Subpack shortcut: NO_WAVE_NO_FOG disables both effects.
#if defined(NO_WAVE_NO_FOG)
  #define NO_WAVE
  #define NO_FOG
#endif

// ---- Sky colors (zenith = top, horizon = middle, edge = bottom band) -------

// Day
#define DAY_ZENITH_COLOR   vec3(0.20, 0.42, 0.85)
#define DAY_HORIZON_COLOR  vec3(0.55, 0.75, 0.96)
#define DAY_EDGE_COLOR     vec3(0.78, 0.88, 0.98)

// Night
#define NIGHT_ZENITH_COLOR   vec3(0.005, 0.01, 0.035)
#define NIGHT_HORIZON_COLOR  vec3(0.03, 0.04, 0.09)
#define NIGHT_EDGE_COLOR     vec3(0.05, 0.06, 0.12)

// Sunrise and sunset share one set of colors
#define DAWN_ZENITH_COLOR   vec3(0.14, 0.20, 0.42)
#define DAWN_HORIZON_COLOR  vec3(0.80, 0.42, 0.30)
#define DAWN_EDGE_COLOR     vec3(1.00, 0.62, 0.30)

// Rain: tints multiplied by the brightness of the clear sky
#define RAIN_ZENITH_COLOR   vec3(0.80, 0.86, 0.95)
#define RAIN_HORIZON_COLOR  vec3(0.88, 0.92, 0.98)

// The End
#define END_ZENITH_COLOR   vec3(0.02, 0.0, 0.05)
#define END_HORIZON_COLOR  vec3(0.30, 0.10, 0.40)

// ---- Fog (distance as a fraction of the render distance) -------------------

#define FOG_LOW   0.1   // fog starts here (rain pulls the range closer automatically)
#define FOG_HIGH  1.0   // fog is full here
#define GODRAY_STRENGTH 0.6   // fan rays at sunrise and sunset (0.0 = off)

// ---- Glow ------------------------------------------------------------------

#define GLOW_INTENSITY 1.0   // brightness of glowing texture pixels (alpha 252/255 and 253/255)

// ---- Waves -----------------------------------------------------------------

#define WAVE_RANGE         48.0   // distance in blocks where waves stop
#define PLANT_SWAY_STRENGTH 0.08  // blocks
#define WATER_WAVE_HEIGHT   0.05  // blocks

#endif
