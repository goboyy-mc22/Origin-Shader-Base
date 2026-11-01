$input v_color0, v_texcoord0, v_worldPos

#include <bgfx_shader.sh>
#include <Origin/insert.sh>

uniform vec4 StarsColor;

void main() {
    gl_FragColor = starColor(v_color0, StarsColor);
}
