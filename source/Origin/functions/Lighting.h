#ifndef ORIGIN_FUNCTIONS_LIGHTING_H
#define ORIGIN_FUNCTIONS_LIGHTING_H

#include <Origin/functions/Common.h>

// Lighting helpers.

// Apply the light map sample (block light and sky light) to a surface color.
vec3 originApplyLightmap(vec3 color, vec3 lightmap) {
    return color * lightmap;
}

// Ambient occlusion: strength 0.0 = none, 1.0 = full vertex occlusion.
float originAmbientOcclusion(float occlusion, float strength) {
    return mix(1.0, occlusion, strength);
}

// Soft light falloff from a point, e.g. for held torches.
float originPointLight(float dist, float radius) {
    float x = saturate(1.0 - dist / radius);
    return x * x;
}

#endif
