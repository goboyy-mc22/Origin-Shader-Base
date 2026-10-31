#ifndef ORIGIN_FUNCTIONS_WIND_H
#define ORIGIN_FUNCTIONS_WIND_H

#include <Origin/functions/Common.h>

// Wind sway for plants and leaves.

// Vertex offset in blocks. weight is 0.0 for rooted vertices and 1.0 for free tips.
// Strength comes from ORIGIN_WIND_STRENGTH.
vec3 originWindOffset(vec3 worldPos, float time, float weight) {
    float phase = worldPos.x * 0.5 + worldPos.z * 0.7;
    float sway = sin(time * 1.5 + phase) * 0.5 + sin(time * 2.7 + phase * 1.7) * 0.25;
    return vec3(sway, 0.0, sway * 0.6) * (weight * 0.05 * ORIGIN_WIND_STRENGTH);
}

#endif
