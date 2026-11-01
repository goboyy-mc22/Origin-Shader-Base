$input v_color0, v_texcoord0, v_worldPos

#include <bgfx_shader.sh>
#include <Origin/insert.sh>

void main() {
    gl_FragColor = cloudColor(v_color0);
}
