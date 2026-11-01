#ifndef SKY_H
#define SKY_H

#include <bgfx_shader.sh>
#include <Origin/configuration.h>

struct SkyColors {
  vec3 zenith;
  vec3 horizon;
  vec3 edge;
};

SkyColors skyDay() {
  SkyColors s;
  s.zenith = DAY_ZENITH_COLOR;
  s.horizon = DAY_HORIZON_COLOR;
  s.edge = DAY_EDGE_COLOR;
  return s;
}

// Sunrise and sunset
SkyColors skyDawn() {
  SkyColors s;
  s.zenith = DAWN_ZENITH_COLOR;
  s.horizon = DAWN_HORIZON_COLOR;
  s.edge = DAWN_EDGE_COLOR;
  return s;
}

SkyColors skyNight() {
  SkyColors s;
  s.zenith = NIGHT_ZENITH_COLOR;
  s.horizon = NIGHT_HORIZON_COLOR;
  s.edge = NIGHT_EDGE_COLOR;
  return s;
}

// Rain turns the given sky gray-blue and keeps its brightness. rain: 0.0 clear, 1.0 full rain.
SkyColors skyRain(SkyColors clearSky, float rain) {
  float amount = rain * 0.85;
  SkyColors s;
  s.zenith = mix(clearSky.zenith, RAIN_ZENITH_COLOR * dot(clearSky.zenith, vec3(0.2126, 0.7152, 0.0722)), amount);
  s.horizon = mix(clearSky.horizon, RAIN_HORIZON_COLOR * dot(clearSky.horizon, vec3(0.2126, 0.7152, 0.0722)), amount);
  s.edge = mix(clearSky.edge, s.horizon, rain);
  return s;
}

SkyColors skyEnd() {
  SkyColors s;
  s.zenith = END_ZENITH_COLOR;
  s.horizon = END_HORIZON_COLOR;
  s.edge = END_HORIZON_COLOR;
  return s;
}

// Overworld sky for the current time and weather.
// day: 0.0 night to 1.0 day, dawn: 0.0 to 1.0 (sunrise or sunset), rain: 0.0 to 1.0
SkyColors skyOverworld(float day, float dawn, float rain) {
  float dayMix = smoothstep(0.05, 0.6, day);
  SkyColors night = skyNight();
  SkyColors noon = skyDay();
  SkyColors dusk = skyDawn();

  SkyColors s;
  s.zenith = mix(mix(night.zenith, noon.zenith, dayMix), dusk.zenith, dawn);
  s.horizon = mix(mix(night.horizon, noon.horizon, dayMix), dusk.horizon, dawn);
  s.edge = mix(mix(night.edge, noon.edge, dayMix), dusk.edge, dawn);
  return skyRain(s, rain);
}

// Color on the sky dome. t: 0.0 at the top (zenith), 1.0 at the bottom (edge).
vec3 skyGradient(SkyColors s, float t) {
  vec3 color = mix(s.zenith, s.horizon, saturate(t * 1.5));
  float t2 = t * t;
  return mix(color, s.edge, t2 * t2);
}

// The End: gradient from the zenith to the horizon. dir: normalized view direction.
vec3 skyEndGradient(vec3 dir) {
  SkyColors s = skyEnd();
  float h = 1.0 - abs(dir.y);
  return mix(s.zenith, s.horizon, h * h);
}

#endif
