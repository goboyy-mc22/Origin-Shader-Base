#ifndef ORIGIN_FUNCTIONS_FOG_H
#define ORIGIN_FUNCTIONS_FOG_H

// Fog
// Fog blending and fog curves.

#include <Origin/functions/Common.h>

// Blend a color towards the fog color. fogAmount is 0.0 (clear) to 1.0 (fully fogged).
// The result respects ORIGIN_FOG_STRENGTH from Configuration.h.
vec3 originApplyFog(vec3 color, vec3 fogColor, float fogAmount) {
    return mix(color, fogColor, clamp(fogAmount * ORIGIN_FOG_STRENGTH, 0.0, 1.0));
}

// Linear fog: 0.0 before start, 1.0 after end.
float originFogLinear(float dist, float start, float end) {
    return saturate((dist - start) / (end - start));
}

// Exponential fog: grows quickly at first and flattens out.
float originFogExp(float dist, float density) {
    return 1.0 - exp(-density * dist);
}

#endif
