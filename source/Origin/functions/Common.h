#ifndef ORIGIN_FUNCTIONS_COMMON_H
#define ORIGIN_FUNCTIONS_COMMON_H

// Shared constants and helpers used by the other Origin function headers.

#include <bgfx_shader.sh>
#include <Origin/Configuration.h>

#define ORIGIN_PI 3.14159265
#define ORIGIN_TAU 6.28318531

// Perceived brightness of a linear RGB color (Rec. 709 weights).
float originLuma(vec3 color) {
    return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

float originSq(float x) {
    return x * x;
}

// Map x from [inMin, inMax] to [outMin, outMax], clamped.
float originRemap(float x, float inMin, float inMax, float outMin, float outMax) {
    return mix(outMin, outMax, saturate((x - inMin) / (inMax - inMin)));
}

#endif
