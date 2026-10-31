#ifndef ORIGIN_FUNCTIONS_LIGHTING_H
#define ORIGIN_FUNCTIONS_LIGHTING_H

// Lighting
// Light map, occlusion, entity overlay and enchantment glint.

#include <Origin/functions/Common.h>

// ---- Light map and occlusion --------------------------------------

// Apply the light map sample (block light and sky light) to a surface color. Tint from ORIGIN_LIGHT_TINT.
vec3 originApplyLightmap(vec3 color, vec3 lightmap) {
    return color * (lightmap * ORIGIN_LIGHT_TINT);
}

// Ambient occlusion: strength 0.0 = none, 1.0 = full vertex occlusion.
float originAmbientOcclusion(float occlusion, float strength) {
    return mix(1.0, occlusion, strength);
}

// Soft light falloff from a point, e.g. for held torches.
float originPointLight(float dist, float radius) {
    float x = saturate(1.0 - dist / radius);
    return x * x;
}

// ---- Entity overlay -----------------------------------------------

// Blend a color with the overlay color (hurt flash, creeper fuse, ...). overlay.a is the amount.
vec3 originApplyOverlay(vec3 color, vec4 overlay) {
    return mix(color, overlay.rgb, overlay.a);
}

// ---- Enchantment glint --------------------------------------------

// Add a glint layer on top of a color. strength: 0.0 = none, 1.0 = full.
vec3 originGlintBlend(vec3 color, vec3 glint, float strength) {
    return color + glint * strength;
}

#endif
