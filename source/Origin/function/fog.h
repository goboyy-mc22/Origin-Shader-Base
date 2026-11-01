#ifndef FOG_H
#define FOG_H

#include <Origin/function/common.h>
#include <Origin/function/detection.h>
#include <Origin/function/sky.h>

// Fog amount in [0, 1]. relativeDist = distance / render distance.
float fogFade(float relativeDist, Environment env, vec2 fogControl) {
  // Vanilla linear fog underwater, in the Nether and in The End
  if (env.underwater || env.nether || env.end) {
    return saturate((relativeDist - fogControl.x) / (fogControl.y - fogControl.x));
  }

#ifdef NO_FOG
  return 0.0;
#else
  // rain pulls the fog range closer
  float low = FOG_LOW * (1.0 - 0.7 * env.rain);
  float high = FOG_HIGH * (1.0 - 0.3 * env.rain);
  float fade = smoothstep(low, high, relativeDist);

  // Thin mist close to the camera
  fade += (1.0 - fade) * 0.3 * (1.0 - exp(-relativeDist * relativeDist * 4.0));
  return saturate(fade);
#endif
}

// Fog color and amount. The color matches the edge of the sky so terrain blends into it.
vec4 computeFog(float relativeDist, Environment env, vec3 fogControl) {
  vec3 color = env.fogColor;
  if (env.end) {
    color = END_HORIZON_COLOR;
  } else if (!env.nether) {
    color = overworldSkyColors(env).edge;
  }
  return vec4(color, fogFade(relativeDist, env, fogControl.xy));
}

// Same, detecting the environment from the vanilla fog uniforms.
vec4 computeFogFromUniforms(float relativeDist, vec3 fogColor, vec3 fogControl) {
  return computeFog(relativeDist, detectEnvironment(fogColor, fogControl), fogControl);
}

vec3 blendFog(vec3 color, vec3 fogColor, float amount) {
  return mix(color, fogColor, saturate(amount));
}

// Fan-shaped light rays toward the sunrise and sunset horizon, returned as extra fog amount.
// worldPos: vertex position, skyLight: sky light of the vertex (0..1), t: time in seconds.
float godRay(Environment env, vec3 worldPos, float skyLight, float relativeDist, float t) {
#if defined(NO_FOG)
  return 0.0;
#else
  if (env.end || env.nether || env.underwater) {
    return 0.0;
  }

  vec3 d = normalize(worldPos);
  float angle = atan2(d.z, d.y);                 // angle around the east-west axis
  float stripes = sin(7.0 * angle + 0.12 * t) * sin(3.0 * angle - 0.08 * t);
  stripes *= stripes;

  float facing = d.x * d.x;                      // strongest toward east and west
  float volume = stripes * facing * (1.0 - facing * facing) * skyLight;
  volume *= relativeDist * relativeDist;
  volume *= env.dawn * (1.0 - env.rain);
  return GODRAY_STRENGTH * smoothstep(0.0, 0.1, volume);
#endif
}

#endif
