#ifndef ORIGIN_FUNCTIONS_SUNMOON_H
#define ORIGIN_FUNCTIONS_SUNMOON_H

#include <Origin/functions/Common.h>

// Sun and moon.

// Tint the sun or moon texture. Brightness comes from ORIGIN_SUNMOON_BRIGHTNESS.
vec4 originSunMoonColor(vec4 tint, vec4 textureColor) {
    vec4 color = tint * textureColor;
    return vec4(color.rgb * ORIGIN_SUNMOON_BRIGHTNESS, color.a);
}

#endif
