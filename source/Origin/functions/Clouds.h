#ifndef ORIGIN_FUNCTIONS_CLOUDS_H
#define ORIGIN_FUNCTIONS_CLOUDS_H

// Clouds
// Cloud color, procedural density and shading.

#include <Origin/functions/Common.h>
#include <Origin/functions/Noise.h>

// Final cloud color. Opacity comes from ORIGIN_CLOUD_OPACITY.
vec4 originCloudColor(vec4 color) {
    return vec4(color.rgb, color.a * ORIGIN_CLOUD_OPACITY);
}

// Procedural cloud density in [0, 1]. coverage: 0.0 = overcast, 1.0 = clear sky.
float originCloudDensity(vec2 position, float time, float coverage) {
    float n = originFbm(position * 0.05 + vec2(time * 0.01, 0.0));
    return smoothstep(coverage, coverage + 0.2, n);
}

// Darken cloud bottoms: density 0.0 = lit color, 1.0 = shadow color.
vec3 originCloudShade(vec3 litColor, vec3 shadowColor, float density) {
    return mix(litColor, shadowColor, density);
}

#endif
