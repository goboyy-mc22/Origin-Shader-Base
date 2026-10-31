#ifndef ORIGIN_FUNCTIONS_GLINT_H
#define ORIGIN_FUNCTIONS_GLINT_H

#include <Origin/functions/Common.h>

// Enchantment glint.

// Add a glint layer on top of a color. strength: 0.0 = none, 1.0 = full.
vec3 originGlintBlend(vec3 color, vec3 glint, float strength) {
    return color + glint * strength;
}

#endif
