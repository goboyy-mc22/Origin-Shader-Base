#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// Sky colors (zenith = top, horizon = middle, edge = lowest band)

#define DAY_ZENITH_COLOR   vec3(0.20, 0.42, 0.85)
#define DAY_HORIZON_COLOR  vec3(0.55, 0.75, 0.96)
#define DAY_EDGE_COLOR     vec3(0.78, 0.88, 0.98)

// Sunrise and sunset share these colors
#define DAWN_ZENITH_COLOR   vec3(0.14, 0.20, 0.42)
#define DAWN_HORIZON_COLOR  vec3(0.80, 0.42, 0.30)
#define DAWN_EDGE_COLOR     vec3(1.00, 0.62, 0.30)

#define NIGHT_ZENITH_COLOR   vec3(0.005, 0.01, 0.035)
#define NIGHT_HORIZON_COLOR  vec3(0.03, 0.04, 0.09)
#define NIGHT_EDGE_COLOR     vec3(0.05, 0.06, 0.12)

// Rain colors are multiplied by the brightness of the sky they replace
#define RAIN_ZENITH_COLOR   vec3(0.80, 0.86, 0.95)
#define RAIN_HORIZON_COLOR  vec3(0.88, 0.92, 0.98)

#define END_ZENITH_COLOR   vec3(0.02, 0.0, 0.05)
#define END_HORIZON_COLOR  vec3(0.30, 0.10, 0.40)

#endif
