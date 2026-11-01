$input a_texcoord0, a_position
#ifdef INSTANCING
    $input i_data1, i_data2, i_data3
#endif

$output v_posTime, v_texcoord0

#include <bgfx_shader.sh>

uniform vec4 ViewPositionAndTime;

void main() {
#ifdef INSTANCING
    mat4 model = mtxFromCols(i_data1, i_data2, i_data3, vec4(0.0, 0.0, 0.0, 1.0));
    vec3 pos = instMul(model, vec4(a_position, 1.0)).xyz;
#else
    vec3 pos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
#endif

    // Rotate the sky: one full turn per hour
    float angle = 0.00174532925 * ViewPositionAndTime.w;
    float s = sin(angle);
    float c = cos(angle);
    pos.xz = mul(mtxFromRows(vec2(c, -s), vec2(s, c)), pos.xz);

    v_texcoord0 = 2.0 * a_texcoord0;
    v_posTime = vec4(-pos.x, pos.y, -pos.z, ViewPositionAndTime.w);
    gl_Position = mul(u_viewProj, vec4(pos, 1.0));
}
