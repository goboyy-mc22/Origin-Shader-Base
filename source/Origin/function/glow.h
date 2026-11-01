#ifndef GLOW_H
#define GLOW_H

#include <Origin/function/common.h>

// Glow from the texture alpha channel. A resource pack marks glowing pixels by setting their alpha:
//   252/255 = full glow, 253/255 = partial glow (40%).
// All other pixels are untouched. Use texture2DLod(..., 0.0) to avoid mipmap blending of the alpha.
vec3 glowFromTexture(vec4 texel) {
  if (texel.a > 0.988 && texel.a < 0.993) {
    float amount = texel.a > 0.989 ? 0.4 : 1.0;
    return texel.rgb * texel.rgb * (amount * GLOW_INTENSITY);
  }
  return vec3_splat(0.0);
}

#endif
