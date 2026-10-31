#ifndef ORIGIN_FUNCTIONS_COMMON_H
#define ORIGIN_FUNCTIONS_COMMON_H

// Shared helpers used by the other Origin function headers.

#include <bgfx_shader.sh>
#include <Origin/Configuration.h>

// Perceived brightness of a linear RGB color (Rec. 709 weights).
float originLuma(vec3 color) {
    return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

#endif
