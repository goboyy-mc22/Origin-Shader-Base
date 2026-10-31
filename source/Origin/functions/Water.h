#ifndef ORIGIN_FUNCTIONS_WATER_H
#define ORIGIN_FUNCTIONS_WATER_H

// Water
// Water shading: fresnel, depth tint and reflection. Wave height is in Wave.h.

#include <Origin/functions/Common.h>

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
