#ifndef ORIGIN_FUNCTIONS_WAVE_H
#define ORIGIN_FUNCTIONS_WAVE_H

// Wave
// Vertex motion: procedural water waves and wind sway for plants and leaves.

#include <Origin/functions/Common.h>
#include <Origin/functions/Noise.h>

// ---- Water waves --------------------------------------------------

// Animated wave height in roughly [-0.5, 0.5], scaled by ORIGIN_WATER_WAVE_STRENGTH.
float originWaterWave(vec2 position, float time) {
    float broad = originNoise(position * 0.8 + vec2(time * 0.6, time * 0.4));
    float fine = originNoise(position * 1.7 - vec2(time * 0.5, time * 0.7));
    return ((broad + fine * 0.5) / 1.5 - 0.5) * ORIGIN_WATER_WAVE_STRENGTH;
}

// ---- Wind ---------------------------------------------------------

// Vertex offset in blocks. weight is 0.0 for rooted vertices and 1.0 for free tips.
// Strength comes from ORIGIN_WIND_STRENGTH.
vec3 originWindOffset(vec3 worldPos, float time, float weight) {
    float phase = worldPos.x * 0.5 + worldPos.z * 0.7;
    float sway = sin(time * 1.5 + phase) * 0.5 + sin(time * 2.7 + phase * 1.7) * 0.25;
    return vec3(sway, 0.0, sway * 0.6) * (weight * 0.05 * ORIGIN_WIND_STRENGTH);
}

#endif
