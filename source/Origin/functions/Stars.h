#ifndef ORIGIN_FUNCTIONS_STARS_H
#define ORIGIN_FUNCTIONS_STARS_H

#include <Origin/functions/Common.h>

// Stars.

// Final star color. Brightness comes from ORIGIN_STAR_BRIGHTNESS.
vec4 originStarColor(vec4 vertexColor, vec4 starsColor) {
    return vec4(vertexColor.rgb * starsColor.rgb * vertexColor.a * ORIGIN_STAR_BRIGHTNESS, vertexColor.a);
}

// Slow brightness flicker in [0.5, 1.0]. seed should differ per star (e.g. a texcoord).
float originStarTwinkle(float seed, float time) {
    return 0.75 + 0.25 * sin(time * 2.0 + seed * ORIGIN_TAU);
}

#endif
