#ifndef ORIGIN_FUNCTIONS_WEATHER_H
#define ORIGIN_FUNCTIONS_WEATHER_H

// Weather
// Rain and snow.

#include <Origin/functions/Common.h>

// Final weather particle color. Opacity comes from ORIGIN_RAIN_OPACITY.
vec4 originWeatherColor(vec4 color) {
    return vec4(color.rgb, color.a * ORIGIN_RAIN_OPACITY);
}

// Apply the weather light texture sample to a particle color.
vec3 originWeatherLight(vec3 color, vec3 light) {
    return color * light;
}

#endif
