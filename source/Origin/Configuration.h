#ifndef ORIGIN_CONFIGURATION_H
#define ORIGIN_CONFIGURATION_H

// Origin Shader Base - configuration
// Every option has a neutral default that matches vanilla rendering.
// Include this file through the headers in Origin/functions/, not directly from materials.

// ---- Fog ----
// Fog strength multiplier. 1.0 = vanilla, 0.0 = no fog, above 1.0 = denser fog.
#define ORIGIN_FOG_STRENGTH 1.0

// ---- Sky ----
// Brightness multiplier of the sun and moon textures.
#define ORIGIN_SUNMOON_BRIGHTNESS 1.0
// Brightness multiplier of the stars.
#define ORIGIN_STAR_BRIGHTNESS 1.0

// ---- Clouds ----
// Opacity multiplier of the clouds.
#define ORIGIN_CLOUD_OPACITY 1.0

// ---- Weather ----
// Opacity multiplier of rain and snow.
#define ORIGIN_RAIN_OPACITY 1.0

// ---- Water ----
// Height multiplier of the procedural water waves (see functions/Water.h).
#define ORIGIN_WATER_WAVE_STRENGTH 1.0

// ---- Wind ----
// Sway multiplier for plants and leaves (see functions/Wind.h).
#define ORIGIN_WIND_STRENGTH 1.0

// ---- Color grading (see functions/Color.h) ----
#define ORIGIN_EXPOSURE 1.0
#define ORIGIN_SATURATION 1.0
#define ORIGIN_CONTRAST 1.0

#endif
