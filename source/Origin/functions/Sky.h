#ifndef ORIGIN_FUNCTIONS_SKY_H
#define ORIGIN_FUNCTIONS_SKY_H

// Sky
// Sky dome, sun and moon, stars and clouds.

#include <Origin/functions/Core.h>

// ---- Sky dome ----------------------------------------------------

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

// ---- Sun and moon ------------------------------------------------

// Tint the sun or moon texture. Brightness comes from ORIGIN_SUNMOON_BRIGHTNESS.
vec4 originSunMoonColor(vec4 tint, vec4 textureColor) {
    vec4 color = tint * textureColor;
    return vec4(color.rgb * ORIGIN_SUNMOON_BRIGHTNESS, color.a);
}

// ---- Stars -------------------------------------------------------

// Final star color. Brightness comes from ORIGIN_STAR_BRIGHTNESS.
vec4 originStarColor(vec4 vertexColor, vec4 starsColor) {
    return vec4(vertexColor.rgb * starsColor.rgb * vertexColor.a * ORIGIN_STAR_BRIGHTNESS, vertexColor.a);
}

// Slow brightness flicker in [0.5, 1.0]. seed should differ per star (e.g. a texcoord).
float originStarTwinkle(float seed, float time) {
    return 0.75 + 0.25 * sin(time * 2.0 + seed * ORIGIN_TAU);
}

// ---- Clouds ------------------------------------------------------

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
