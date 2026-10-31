#ifndef ORIGIN_FUNCTIONS_ACTOR_H
#define ORIGIN_FUNCTIONS_ACTOR_H

#include <Origin/functions/Common.h>

// Entities and held items.

// Blend a color with the overlay color (hurt flash, creeper fuse, ...). overlay.a is the amount.
vec3 originApplyOverlay(vec3 color, vec4 overlay) {
    return mix(color, overlay.rgb, overlay.a);
}

#endif
