#ifndef ORIGIN_FUNCTIONS_SKY_H
#define ORIGIN_FUNCTIONS_SKY_H

#include <Origin/functions/Common.h>

// Sky dome gradient. horizon is 0.0 at the zenith and 1.0 at the horizon.
vec4 originSkyGradient(vec4 skyColor, vec4 fogColor, float horizon) {
    return mix(skyColor, fogColor, horizon);
}

#endif
