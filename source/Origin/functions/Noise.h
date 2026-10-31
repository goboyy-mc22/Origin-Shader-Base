#ifndef ORIGIN_FUNCTIONS_NOISE_H
#define ORIGIN_FUNCTIONS_NOISE_H

#include <Origin/functions/Common.h>

// Procedural noise. Used by clouds, water and other animated effects.

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

#endif
