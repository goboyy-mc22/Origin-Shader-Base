$input a_color0, a_position, a_texcoord0
#ifdef INSTANCING
    $input i_data1, i_data2, i_data3
#endif

$output v_color0, v_texcoord0, v_worldPos

#include <bgfx_shader.sh>
#include <Origin/insert.sh>

uniform vec4 SkyColor;
uniform vec4 FogColor;
uniform vec4 FogAndDistanceControl;

void main() {
    vec3 worldPosition;
#ifdef INSTANCING
    mat4 model = mtxFromCols(i_data1, i_data2, i_data3, vec4(0.0, 0.0, 0.0, 1.0));
    worldPosition = instMul(model, vec4(a_position, 1.0)).xyz;
#else
    worldPosition = mul(u_model[0], vec4(a_position, 1.0)).xyz;
#endif

    SkyColors sky = skyOverworld(detectDay(FogColor.rgb), detectDawn(FogColor.rgb), detectRain(FogAndDistanceControl.xyz));
    vec3 skyColor = skyGradient(sky, a_color0.x);
    if (detectUnderwater(FogColor.rgb, FogAndDistanceControl.xy)) {
        skyColor = FogColor.rgb;
    }
    v_color0 = vec4(skyColor, mix(SkyColor.a, FogColor.a, a_color0.x));
    v_texcoord0 = a_texcoord0;
    v_worldPos = worldPosition;
    gl_Position = mul(u_viewProj, vec4(worldPosition, 1.0));
}
