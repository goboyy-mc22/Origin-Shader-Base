#ifndef ORIGIN_FUNCTIONS_WAVE_H
#define ORIGIN_FUNCTIONS_WAVE_H

// Wave
// Vertex motion: water waves and leaf sway, plus the checks that decide which vertices move.
// Both effects are off by default, see Configuration.h.

#include <Origin/functions/Common.h>
#include <Origin/functions/Noise.h>

// ---- Detection ----------------------------------------------------

// True for translucent water vertices. This is the same test vanilla uses to fade water with distance.
bool originIsWater(vec4 vertexColor) {
    return vertexColor.a < 0.95;
}

// True when the vertex has a biome tint (the color is not gray), as on leaves and grass.
bool originIsTinted(vec4 vertexColor) {
    return abs(vertexColor.r - vertexColor.g) > 0.02 || abs(vertexColor.g - vertexColor.b) > 0.02;
}

// True when the vertex sits on a block corner. Full cubes such as leaves only have corner vertices,
// while plants and flowers have vertices inside the block, so they are left alone.
bool originIsBlockCorner(vec3 localPos) {
    vec3 offset = abs(fract(localPos + vec3_splat(0.5)) - vec3_splat(0.5));
    return max(offset.x, max(offset.y, offset.z)) < 0.001;
}

// ---- Fade ---------------------------------------------------------

// 1.0 near the camera, fading to 0.0 at ORIGIN_WAVE_RANGE.
float originWaveFade(float camDist) {
    return 1.0 - smoothstep(0.5, 1.0, saturate(camDist / ORIGIN_WAVE_RANGE));
}

// ---- Water waves --------------------------------------------------

// Wave shape in roughly [-0.5, 0.5].
float originWaterWave(vec2 position, float time) {
    float broad = originNoise(position * 0.8 + vec2(time * 0.6, time * 0.4));
    float fine = originNoise(position * 1.7 - vec2(time * 0.5, time * 0.7));
    return (broad + fine * 0.5) / 1.5 - 0.5;
}

// Vertical offset in blocks for a water vertex. Height, speed and scale come from Configuration.h.
float originWaterWaveOffset(vec3 worldPos, float time) {
    vec2 position = worldPos.xz * ORIGIN_WATER_WAVE_SCALE;
    return originWaterWave(position, time * ORIGIN_WATER_WAVE_SPEED) * ORIGIN_WATER_WAVE_HEIGHT;
}

// ---- Leaf sway ----------------------------------------------------

// Position offset in blocks for a leaf vertex. Strength and speed come from Configuration.h.
// The offset only depends on the world position, so neighbouring leaf blocks move together without gaps.
vec3 originLeafSway(vec3 worldPos, float time) {
    float t = time * ORIGIN_LEAF_SWAY_SPEED;
    float phase = worldPos.x * 0.5 + worldPos.z * 0.7 + worldPos.y * 0.3;
    float sway = sin(t * 1.5 + phase) * 0.5 + sin(t * 2.7 + phase * 1.7) * 0.25;
    float lift = sin(t * 1.9 + phase * 1.3) * 0.3;
    return vec3(sway, lift, sway * 0.6) * ORIGIN_LEAF_SWAY_STRENGTH;
}

#endif
