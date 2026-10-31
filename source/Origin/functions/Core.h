#ifndef ORIGIN_FUNCTIONS_CORE_H
#define ORIGIN_FUNCTIONS_CORE_H

// Core
// Math helpers, procedural noise and color grading shared by every other Origin function header.

#include <bgfx_shader.sh>
#include <Origin/Configuration.h>

// ---- Math --------------------------------------------------------

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

// ---- Noise -------------------------------------------------------

// Hash without sine, stable on mobile GPUs. Returns a value in [0, 1).
float originHash(vec2 p) {
    vec3 p3 = fract(vec3(p.x, p.y, p.x) * 0.1031);
    p3 = p3 + vec3_splat(dot(p3, vec3(p3.y, p3.z, p3.x) + vec3_splat(33.33)));
    return fract((p3.x + p3.y) * p3.z);
}

// Smooth value noise in [0, 1].
float originNoise(vec2 p) {
    vec2 cell = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (vec2_splat(3.0) - 2.0 * f);

    float a = originHash(cell);
    float b = originHash(cell + vec2(1.0, 0.0));
    float c = originHash(cell + vec2(0.0, 1.0));
    float d = originHash(cell + vec2(1.0, 1.0));
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

// Fractal noise, 4 octaves, in [0, 1].
float originFbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;

    value += amplitude * originNoise(p);
    p = p * 2.02 + vec2(17.1, 9.2);
    amplitude *= 0.5;

    value += amplitude * originNoise(p);
    p = p * 2.02 + vec2(17.1, 9.2);
    amplitude *= 0.5;

    value += amplitude * originNoise(p);
    p = p * 2.02 + vec2(17.1, 9.2);
    amplitude *= 0.5;

    value += amplitude * originNoise(p);
    return value / 0.9375;
}

// ---- Color -------------------------------------------------------

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
