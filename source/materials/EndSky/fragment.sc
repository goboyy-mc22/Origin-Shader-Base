$input v_color0, v_texcoord0, v_worldPos

#include <bgfx_shader.sh>
#include <Origin/insert.sh>

SAMPLER2D_AUTOREG(s_SkyTexture);
uniform vec4 SkyColor;

void main() {
    vec4 stars = SkyColor * texture2D(s_SkyTexture, v_texcoord0);
    gl_FragColor = vec4(skyEndGradient(normalize(v_worldPos)) + 0.5 * stars.rgb, 1.0);
}
