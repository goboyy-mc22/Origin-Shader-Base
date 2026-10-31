#ifndef ORIGIN_FUNCTIONS_WORLD_H
#define ORIGIN_FUNCTIONS_WORLD_H

// World
// Fog, lighting, water, wind, weather, entities and enchantment glint.

#include <Origin/functions/Core.h>

// ---- Fog ---------------------------------------------------------

// Blend a color towards the fog color. fogAmount is 0.0 (clear) to 1.0 (fully fogged).
// The result respects ORIGIN_FOG_STRENGTH from Configuration.h.
vec3 originApplyFog(vec3 color, vec3 fogColor, float fogAmount) {
    return mix(color, fogColor, clamp(fogAmount * ORIGIN_FOG_STRENGTH, 0.0, 1.0));
}

// Linear fog: 0.0 before start, 1.0 after end.
float originFogLinear(float dist, float start, float end) {
    return saturate((dist - start) / (end - start));
}

// Exponential fog: grows quickly at first and flattens out.
float originFogExp(float dist, float density) {
    return 1.0 - exp(-density * dist);
}

// ---- Lighting ----------------------------------------------------

// Apply the light map sample (block light and sky light) to a surface color.
vec3 originApplyLightmap(vec3 color, vec3 lightmap) {
    return color * lightmap;
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

// ---- Water -------------------------------------------------------

// Animated wave height in roughly [-0.5, 0.5], scaled by ORIGIN_WATER_WAVE_STRENGTH.
float originWaterWave(vec2 position, float time) {
    float broad = originNoise(position * 0.8 + vec2(time * 0.6, time * 0.4));
    float fine = originNoise(position * 1.7 - vec2(time * 0.5, time * 0.7));
    return ((broad + fine * 0.5) / 1.5 - 0.5) * ORIGIN_WATER_WAVE_STRENGTH;
}

// Schlick fresnel. cosTheta is the dot of the view direction and the surface normal.
float originFresnel(float cosTheta, float f0) {
    return f0 + (1.0 - f0) * pow(1.0 - saturate(cosTheta), 5.0);
}

// Tint water towards a deep color with depth (in blocks).
vec3 originWaterTint(vec3 shallowColor, vec3 deepColor, float depth) {
    return mix(shallowColor, deepColor, 1.0 - exp(-depth * 0.25));
}

// Mix the water color with a reflection (e.g. the sky color) by fresnel.
vec3 originWaterReflect(vec3 waterColor, vec3 reflectionColor, float fresnel) {
    return mix(waterColor, reflectionColor, fresnel);
}

// ---- Wind --------------------------------------------------------

// Vertex offset in blocks. weight is 0.0 for rooted vertices and 1.0 for free tips.
// Strength comes from ORIGIN_WIND_STRENGTH.
vec3 originWindOffset(vec3 worldPos, float time, float weight) {
    float phase = worldPos.x * 0.5 + worldPos.z * 0.7;
    float sway = sin(time * 1.5 + phase) * 0.5 + sin(time * 2.7 + phase * 1.7) * 0.25;
    return vec3(sway, 0.0, sway * 0.6) * (weight * 0.05 * ORIGIN_WIND_STRENGTH);
}

// ---- Weather -----------------------------------------------------

// Final weather particle color. Opacity comes from ORIGIN_RAIN_OPACITY.
vec4 originWeatherColor(vec4 color) {
    return vec4(color.rgb, color.a * ORIGIN_RAIN_OPACITY);
}

// Apply the weather light texture sample to a particle color.
vec3 originWeatherLight(vec3 color, vec3 light) {
    return color * light;
}

// ---- Entities and held items -------------------------------------

// Blend a color with the overlay color (hurt flash, creeper fuse, ...). overlay.a is the amount.
vec3 originApplyOverlay(vec3 color, vec4 overlay) {
    return mix(color, overlay.rgb, overlay.a);
}

// ---- Enchantment glint -------------------------------------------

// Add a glint layer on top of a color. strength: 0.0 = none, 1.0 = full.
vec3 originGlintBlend(vec3 color, vec3 glint, float strength) {
    return color + glint * strength;
}

#endif
