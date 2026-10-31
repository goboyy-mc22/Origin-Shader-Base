#ifndef ORIGIN_CONFIGURATION_H
#define ORIGIN_CONFIGURATION_H

// Origin Shader Base - configuration
//
// Every option has a neutral default that matches vanilla rendering.
// Colors are multipliers applied on top of the colors the game provides
// (time of day, weather, biome), so vec3(1.0, 1.0, 1.0) means "unchanged".
// Effects that move geometry are switched on by removing the // in front of their #define.

// ==== Sky =========================================================
// The sky dome is a gradient from the zenith (top) to the horizon (middle) and the edge (very bottom).
// Tint of each band:
#define ORIGIN_SKY_ZENITH_TINT  vec3(1.0, 1.0, 1.0)
#define ORIGIN_SKY_HORIZON_TINT vec3(1.0, 1.0, 1.0)
#define ORIGIN_SKY_EDGE_TINT    vec3(1.0, 1.0, 1.0)
// Where the horizon band sits on the dome: 0.0 = zenith, 1.0 = edge.
// 1.0 is the vanilla two-color gradient (the edge band is not visible).
#define ORIGIN_SKY_HORIZON_POSITION 1.0

// Brightness multipliers of the sun, moon and stars.
#define ORIGIN_SUNMOON_BRIGHTNESS 1.0
#define ORIGIN_STAR_BRIGHTNESS 1.0

// ==== Fog =========================================================
// Fog strength: 1.0 = vanilla, 0.0 = no fog, above 1.0 = denser fog.
#define ORIGIN_FOG_STRENGTH 1.0
// Tint of the fog color.
#define ORIGIN_FOG_TINT vec3(1.0, 1.0, 1.0)

// ==== Clouds ======================================================
#define ORIGIN_CLOUD_TINT vec3(1.0, 1.0, 1.0)
// Opacity multiplier of the clouds.
#define ORIGIN_CLOUD_OPACITY 1.0

// ==== Lighting ====================================================
// Tint of the light map (block light and sky light).
#define ORIGIN_LIGHT_TINT vec3(1.0, 1.0, 1.0)

// ==== Weather =====================================================
// Opacity multiplier of rain and snow.
#define ORIGIN_RAIN_OPACITY 1.0

// ==== Water waves =================================================
// Moves the water surface up and down. Remove the // to enable.
//#define ORIGIN_ENABLE_WATER_WAVES
// Wave height in blocks.
#define ORIGIN_WATER_WAVE_HEIGHT 0.06
// Animation speed multiplier.
#define ORIGIN_WATER_WAVE_SPEED 1.0
// Wave size: higher values give smaller, more frequent waves.
#define ORIGIN_WATER_WAVE_SCALE 1.0

// ==== Leaf sway ===================================================
// Sways tree leaves in the wind. Remove the // to enable.
//#define ORIGIN_ENABLE_LEAF_SWAY
// Sway distance in blocks.
#define ORIGIN_LEAF_SWAY_STRENGTH 0.05
// Animation speed multiplier.
#define ORIGIN_LEAF_SWAY_SPEED 1.0

// Water waves and leaf sway fade out with distance. Distance in blocks where they stop.
#define ORIGIN_WAVE_RANGE 64.0

// ==== Color grading (see functions/Color.h) =======================
#define ORIGIN_EXPOSURE 1.0
#define ORIGIN_SATURATION 1.0
#define ORIGIN_CONTRAST 1.0

#endif
