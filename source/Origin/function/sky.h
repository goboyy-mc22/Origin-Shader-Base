#ifndef SKY_H
#define SKY_H

#include <Origin/function/common.h>
#include <Origin/function/coloring.h>
#include <Origin/function/detection.h>
#include <Origin/function/noise.h>

struct SkyColors {
  vec3 zenith;
  vec3 horizon;
  vec3 edge;
};

// ---- Sky colors ------------------------------------------------------------

// Overworld: night -> day by brightness, sunrise and sunset merged, then rain and underwater.
SkyColors overworldSkyColors(Environment env) {
  SkyColors s;

  float dayMix = smoothstep(0.05, 0.6, env.day);
  s.zenith = mix(NIGHT_ZENITH_COLOR, DAY_ZENITH_COLOR, dayMix);
  s.horizon = mix(NIGHT_HORIZON_COLOR, DAY_HORIZON_COLOR, dayMix);
  s.edge = mix(NIGHT_EDGE_COLOR, DAY_EDGE_COLOR, dayMix);

  s.zenith = mix(s.zenith, DAWN_ZENITH_COLOR, env.dawn);
  s.horizon = mix(s.horizon, DAWN_HORIZON_COLOR, env.dawn);
  s.edge = mix(s.edge, DAWN_EDGE_COLOR, env.dawn);

  // Rain: gray-blue sky with the brightness of the clear sky
  float rainMix = env.rain * 0.85;
  s.zenith = mix(s.zenith, RAIN_ZENITH_COLOR * luminance(s.zenith), rainMix);
  s.horizon = mix(s.horizon, RAIN_HORIZON_COLOR * luminance(s.horizon), rainMix);
  s.edge = mix(s.edge, s.horizon, env.rain);

  if (env.underwater) {
    vec3 water = env.fogColor * env.fogColor * 2.0;
    s.zenith = water * 0.7;
    s.horizon = water;
    s.edge = water;
  }

  return s;
}

// ---- Overworld sky ---------------------------------------------------------

// dir: view direction, y up. Three color bands plus sun and moon glow.
vec3 renderSky(SkyColors c, Environment env, vec3 dir, vec3 sunDir, vec3 moonDir) {
  float vh = 1.0 - dir.y * dir.y;
  float vh2 = vh * vh;
  float vh4 = vh2 * vh2;
  float edgeBlend = vh4 * vh4;
  float zenithBlend = 0.8 * edgeBlend + 0.2 * vh2;

  vec3 sky = mix(c.horizon, c.edge, edgeBlend);
  sky = mix(c.zenith, sky, zenithBlend);
  sky *= 1.0 + 0.5 * min(dir.y, 0.0); // darker below the horizon

  float outdoors = env.underwater ? 0.0 : 1.0;
  float clearSky = 1.0 - 0.9 * env.rain;

  float sunAmount = saturate(dot(dir, sunDir));
  float s2 = sunAmount * sunAmount;
  float s4 = s2 * s2;
  float s8 = s4 * s4;
  float s16 = s8 * s8;
  float sunVisible = smoothstep(-0.25, 0.05, sunDir.y);
  sky += c.edge * (0.6 * s8 + 2.0 * s16 * s16) * sunVisible * clearSky * outdoors * (1.0 + 1.5 * env.dawn);

  float moonAmount = saturate(dot(dir, moonDir));
  float m2 = moonAmount * moonAmount;
  float m4 = m2 * m2;
  float m8 = m4 * m4;
  float m16 = m8 * m8;
  float moonVisible = smoothstep(-0.25, 0.05, moonDir.y);
  sky += vec3(0.5, 0.6, 0.9) * (0.05 * m8 + 0.4 * m16 * m16) * moonVisible * clearSky * outdoors * (1.0 - env.day);

  return sky;
}

// ---- Star variations (optional) --------------------------------------------

// How visible night objects are: 1.0 at night, 0.0 by day, hidden by rain and underwater.
float nightVisibility(Environment env) {
  float night = 1.0 - smoothstep(0.02, 0.3, env.day);
  return night * (1.0 - env.rain) * (env.underwater ? 0.0 : 1.0);
}

// A shooting star crossing the sky every few seconds.
vec3 shootingStar(vec3 dir, Environment env, float t) {
  float period = 14.0;
  float duration = 1.6;
  float cycle = floor(t / period);
  float life = saturate((t - cycle * period) / duration);
  if (life <= 0.0 || life >= 1.0 || dir.y < 0.0) {
    return vec3_splat(0.0);
  }

  float angle = 6.2831853 * hash31(vec3(cycle, 1.7, 3.1));
  vec2 uv = mul(rotation2d(angle), dir.xz * 6.0);
  float head = mix(-1.5, 1.5, life);
  float trail = smoothstep(head - 0.7, head, uv.x) * (1.0 - smoothstep(head, head + 0.03, uv.x));
  float streakLine = exp(-abs(uv.y + dir.y * 2.0) * 70.0);
  float fade = sin(life * PI);
  return vec3(0.8, 0.9, 1.0) * trail * streakLine * fade * nightVisibility(env);
}

// Dense stars and a faint glowing band across the sky (milky way).
vec3 galaxyStars(vec3 dir, Environment env, float t) {
  float visible = nightVisibility(env);
  if (visible <= 0.0) {
    return vec3_splat(0.0);
  }

  float slow = t * 0.002;
  vec3 d = vec3(mul(rotation2d(slow), dir.xy), dir.z);

  float band = d.x + 0.2 * d.y + 0.1 * sin(4.0 * d.z);
  float glow = exp(-band * band * 14.0);
  float cloud = noise3d(d * 5.0) * noise3d(d * 11.0 + 3.0);

  float h = hash31(floor(d * 170.0));
  float threshold = mix(0.992, 0.95, glow);
  float star = smoothstep(threshold, 1.0, h) * (0.6 + 0.4 * sin(t * 2.0 + h * 60.0));

  vec3 color = vec3(0.8, 0.9, 1.0) * star;
  color += vec3(0.35, 0.3, 0.6) * glow * (0.04 + 0.12 * cloud);
  return color * visible;
}

// ---- The End ---------------------------------------------------------------

// Drifting streaks of light over a purple gradient.
vec3 renderEndSky(vec3 dir, float t) {
  t *= 0.1;
  float angle = atan2(dir.x, dir.z);

  float n1 = 0.5 + 0.5 * sin(3.0 * angle + t + 8.0 * dir.x * dir.y);
  float n2 = 0.5 + 0.5 * sin(5.0 * angle - 0.5 * t + 4.0 * n1);

  float up = 0.5 + 0.5 * dir.y;
  float streak = (0.6 * n1 * n2 + 0.4 * n1) * (1.0 - up * up * up);
  float f = 0.3 * streak + 0.7 * smoothstep(1.0, -0.5, dir.y);

  float s2 = streak * streak;
  float s6 = s2 * s2 * s2;
  vec3 sky = mix(END_ZENITH_COLOR, END_HORIZON_COLOR, f * f);
  sky += (0.1 * streak + 1.5 * s6) * vec3(1.6, 0.5, 0.1);
  return sky;
}

// Decoration: soft colored gas clouds.
vec3 endNebula(vec3 dir, float t) {
  vec3 p = dir * 3.0 + vec3(0.0, 0.0, t * 0.01);
  float a = noise3d(p);
  float b = noise3d(p * 2.3 + 5.0);
  float cloud = smoothstep(0.45, 0.9, a * (0.6 + 0.8 * b));
  vec3 color = mix(vec3(0.45, 0.10, 0.60), vec3(0.10, 0.45, 0.60), b);
  return color * cloud * 0.5;
}

// Decoration: a distant planet with a lit side and a thin atmosphere.
vec3 endPlanet(vec3 dir) {
  vec3 center = normalize(vec3(0.5, 0.35, -0.8));
  float d = dot(dir, center);
  float disc = smoothstep(0.985, 0.988, d);
  float rim = smoothstep(0.975, 0.985, d) * (1.0 - disc);

  vec3 offset = dir - center * d;
  float litAmount = saturate(0.55 + 25.0 * dot(offset, normalize(vec3(-0.6, 0.5, 0.3))));
  return vec3(0.35, 0.55, 0.45) * litAmount * disc + vec3(0.3, 0.5, 0.9) * rim * 0.4;
}

// ---- Sky objects (sun, moon, stars materials) --------------------------------

vec4 sunMoonColor(vec4 tint, vec4 texel) {
  vec4 color = texel;
  color.rgb *= tint.rgb;
  color.rgb *= 4.0 * color.rgb;
  float transparency = 1.0 - tint.a;
  color.a *= 1.0 - transparency * transparency;
  color.rgb = tonemap(color.rgb);
  return color;
}

vec4 starColor(vec4 vertexColor, vec4 starsTint) {
  return vec4(vertexColor.rgb * starsTint.rgb * vertexColor.a, vertexColor.a);
}

#endif
