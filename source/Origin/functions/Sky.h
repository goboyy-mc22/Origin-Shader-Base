#ifndef ORIGIN_FUNCTIONS_SKY_H
#define ORIGIN_FUNCTIONS_SKY_H

// Sky
// Sky dome, sun and moon, and stars.

#include <Origin/functions/Common.h>

// ---- Sky dome -----------------------------------------------------

// Three-band sky color. t is 0.0 at the zenith and 1.0 at the very bottom of the dome (the edge).
// The zenith blends into the horizon up to ORIGIN_SKY_HORIZON_POSITION, then the horizon blends into the edge.
vec3 originSkyColor(vec3 zenith, vec3 horizon, vec3 edge, float t) {
    float position = max(ORIGIN_SKY_HORIZON_POSITION, 0.0001);
    vec3 color = mix(zenith, horizon, saturate(t / position));
    return mix(color, edge, saturate((t - position) / max(1.0 - position, 0.0001)));
}

// Sky dome color from the colors the game provides: skyColor is the zenith, fogColor is the horizon and edge.
// The tints from Configuration.h are applied to each band. t is the vertex gradient (0.0 top, 1.0 bottom).
vec4 originSkyGradient(vec4 skyColor, vec4 fogColor, float t) {
    vec3 rgb = originSkyColor(
        skyColor.rgb * ORIGIN_SKY_ZENITH_TINT,
        fogColor.rgb * ORIGIN_SKY_HORIZON_TINT,
        fogColor.rgb * ORIGIN_SKY_EDGE_TINT,
        t
    );
    return vec4(rgb, mix(skyColor.a, fogColor.a, t));
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
