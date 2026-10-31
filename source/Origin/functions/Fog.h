#ifndef ORIGIN_FUNCTIONS_FOG_H
#define ORIGIN_FUNCTIONS_FOG_H

#include <Origin/functions/Common.h>

// Blend a color towards the fog color. fogAmount is 0.0 (clear) to 1.0 (fully fogged).
// The result respects ORIGIN_FOG_STRENGTH from Configuration.h.
vec3 originApplyFog(vec3 color, vec3 fogColor, float fogAmount) {
    return mix(color, fogColor, clamp(fogAmount * ORIGIN_FOG_STRENGTH, 0.0, 1.0));
}

#endif
