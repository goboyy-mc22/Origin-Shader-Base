#ifndef ORIGIN_FUNCTIONS_WATER_H
#define ORIGIN_FUNCTIONS_WATER_H

#include <Origin/functions/Common.h>
#include <Origin/functions/Noise.h>

// Water.

// Animated wave height in roughly [-0.5, 0.5], scaled by ORIGIN_WATER_WAVE_STRENGTH.
float originWaterWave(vec2 position, float time) {
    float broad = originNoise(position * 0.8 + vec2(time * 0.6, time * 0.4));
    float fine = originNoise(position * 1.7 - vec2(time * 0.5, time * 0.7));
    return ((broad + fine * 0.5) / 1.5 - 0.5) * ORIGIN_WATER_WAVE_STRENGTH;
}

// Schlick fresnel. cosTheta is the dot of the view direction and the surface normal.
float originFresnel(float cosTheta, float f0) {
    return f0 + (1.0 - f0) * pow(1.0 - saturate(cosTheta), 5.0);
}

// Tint water towards a deep color with depth (in blocks).
vec3 originWaterTint(vec3 shallowColor, vec3 deepColor, float depth) {
    return mix(shallowColor, deepColor, 1.0 - exp(-depth * 0.25));
}

// Mix the water color with a reflection (e.g. the sky color) by fresnel.
vec3 originWaterReflect(vec3 waterColor, vec3 reflectionColor, float fresnel) {
    return mix(waterColor, reflectionColor, fresnel);
}

#endif
