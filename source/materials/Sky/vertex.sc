#ifndef INSTANCING
    $input a_color0, a_position
    $output v_worldPos, v_skyState
#endif

#include <bgfx_shader.sh>

#ifndef INSTANCING
    #include <Origin/insert.sh>

    uniform vec4 FogColor;
    uniform vec4 FogAndDistanceControl;
#endif

void main() {
#ifndef INSTANCING
    // x: underwater, y: rain, z: day, w: sunrise/sunset
    v_skyState = vec4(
        float(detectUnderwater(FogColor.rgb, FogAndDistanceControl.xy)),
        detectRain(FogAndDistanceControl.xyz),
        detectDay(FogColor.rgb),
        detectDawn(FogColor.rgb)
    );

    // The sky is drawn as a full screen quad; the view direction is rebuilt per pixel.
    vec4 pos = vec4(a_position.xzy, 1.0);
    pos.xy = 2.0 * clamp(pos.xy, -0.5, 0.5);
    v_worldPos = mul(u_invViewProj, pos).xyz;

    gl_Position = pos;
#else
    gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
#endif
}
