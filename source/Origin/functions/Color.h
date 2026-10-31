#ifndef ORIGIN_FUNCTIONS_COLOR_H
#define ORIGIN_FUNCTIONS_COLOR_H

// Color
// Color space conversion, tonemapping and grading.

#include <Origin/functions/Common.h>

vec3 originToLinear(vec3 color) {
    return pow(color, vec3_splat(2.2));
}

vec3 originToGamma(vec3 color) {
    return pow(color, vec3_splat(1.0 / 2.2));
}

// amount: 0.0 = grayscale, 1.0 = unchanged, above 1.0 = more saturated.
vec3 originSaturation(vec3 color, float amount) {
    return mix(vec3_splat(originLuma(color)), color, amount);
}

// amount: 0.0 = flat gray, 1.0 = unchanged, above 1.0 = more contrast.
vec3 originContrast(vec3 color, float amount) {
    return mix(vec3_splat(0.5), color, amount);
}

vec3 originTonemapReinhard(vec3 color) {
    return color / (vec3_splat(1.0) + color);
}

// Narkowicz ACES filmic approximation.
vec3 originTonemapAces(vec3 color) {
    vec3 numerator = color * (2.51 * color + vec3_splat(0.03));
    vec3 denominator = color * (2.43 * color + vec3_splat(0.59)) + vec3_splat(0.14);
    return saturate(numerator / denominator);
}

// Exposure, saturation and contrast from Configuration.h (neutral by default).
vec3 originGrade(vec3 color) {
    color = color * ORIGIN_EXPOSURE;
    color = originSaturation(color, ORIGIN_SATURATION);
    return originContrast(color, ORIGIN_CONTRAST);
}

#endif
