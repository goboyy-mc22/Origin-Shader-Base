#ifndef ORIGIN_H
#define ORIGIN_H

// Single entry point for materials: #include <Origin/Origin.h>
// Unused functions are removed by the shader compiler, so including everything is free.

#include <Origin/Configuration.h>
#include <Origin/functions/Common.h>
#include <Origin/functions/Noise.h>
#include <Origin/functions/Color.h>
#include <Origin/functions/Sky.h>
#include <Origin/functions/Clouds.h>
#include <Origin/functions/Fog.h>
#include <Origin/functions/Lighting.h>
#include <Origin/functions/Water.h>
#include <Origin/functions/Wave.h>
#include <Origin/functions/Weather.h>

#ifdef ORIGIN_ENABLE_LEAF_SWAY
#error PROBE_LEAF_SWAY_DEFINE_REACHED_SHADER
#endif

#endif
