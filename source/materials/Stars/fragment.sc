$input v_color0, v_texcoord0, v_worldPos

#include <bgfx_shader.sh>
#include <Origin/functions/Sky.h>

uniform vec4 StarsColor;

void main() {
    gl_FragColor = originStarColor(v_color0, StarsColor);
}
