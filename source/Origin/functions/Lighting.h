#ifndef ORIGIN_FUNCTIONS_LIGHTING_H
#define ORIGIN_FUNCTIONS_LIGHTING_H

#include <Origin/functions/Common.h>

// Apply the light map sample (block light and sky light) to a surface color.
vec3 originApplyLightmap(vec3 color, vec3 lightmap) {
    return color * lightmap;
}

#endif
