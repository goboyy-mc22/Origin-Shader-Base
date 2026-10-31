#ifndef ORIGIN_FUNCTIONS_SKY_H
#define ORIGIN_FUNCTIONS_SKY_H

// Sky
// Sky dome, sun and moon, and stars.

#include <Origin/functions/Common.h>

// ---- Sky dome -----------------------------------------------------

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

// ---- Sun and moon -------------------------------------------------

// Tint the sun or moon texture. Brightness comes from ORIGIN_SUNMOON_BRIGHTNESS.
vec4 originSunMoonColor(vec4 tint, vec4 textureColor) {
    vec4 color = tint * textureColor;
    return vec4(color.rgb * ORIGIN_SUNMOON_BRIGHTNESS, color.a);
}

// ---- Stars --------------------------------------------------------

// Final star color. Brightness comes from ORIGIN_STAR_BRIGHTNESS.
vec4 originStarColor(vec4 vertexColor, vec4 starsColor) {
    return vec4(vertexColor.rgb * starsColor.rgb * vertexColor.a * ORIGIN_STAR_BRIGHTNESS, vertexColor.a);
}

// Slow brightness flicker in [0.5, 1.0]. seed should differ per star (e.g. a texcoord).
float originStarTwinkle(float seed, float time) {
    return 0.75 + 0.25 * sin(time * 2.0 + seed * ORIGIN_TAU);
}

#endif
