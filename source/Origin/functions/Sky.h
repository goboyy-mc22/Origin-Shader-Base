#ifndef ORIGIN_FUNCTIONS_SKY_H
#define ORIGIN_FUNCTIONS_SKY_H

#include <Origin/functions/Common.h>

// Sky dome helpers.

// Sky dome gradient. horizon is 0.0 at the zenith and 1.0 at the horizon.
vec4 originSkyGradient(vec4 skyColor, vec4 fogColor, float horizon) {
    return mix(skyColor, fogColor, horizon);
}

// 1.0 when looking at the horizon, 0.0 when looking straight up or down.
float originHorizonFactor(vec3 viewDir) {
    return 1.0 - saturate(abs(normalize(viewDir).y));
}

// Soft glow around the sun. Higher power = tighter glow.
vec3 originSunGlow(vec3 viewDir, vec3 sunDir, vec3 glowColor, float power) {
    float facing = saturate(dot(normalize(viewDir), normalize(sunDir)));
    return glowColor * pow(facing, power);
}

#endif
