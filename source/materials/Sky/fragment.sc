#ifndef INSTANCING
    $input v_worldPos, v_skyState
#endif

#include <bgfx_shader.sh>

#ifndef INSTANCING
    #include <Origin/insert.sh>

    uniform vec4 TimeOfDay;
    uniform vec4 FogColor;
    uniform vec4 ViewPositionAndTime;
#endif

void main() {
#ifndef INSTANCING
    vec3 view = normalize(v_worldPos);
    vec3 dir = vec3(-view.x, view.y, -view.z);

    Environment env;
    env.end = false;
    env.nether = false;
    env.underwater = v_skyState.x > 0.5;
    env.rain = v_skyState.y;
    env.day = v_skyState.z;
    env.dawn = v_skyState.w;
    env.fogColor = FogColor.rgb;

    float t = ViewPositionAndTime.w;
    vec3 sky = renderSky(overworldSkyColors(env), env, dir, sunDirection(TimeOfDay.x), moonDirection(TimeOfDay.x));

#ifdef SHOOTING_STAR
    sky += shootingStar(dir, env, t);
#endif
#ifdef GALAXY_STARS
    sky += galaxyStars(dir, env, t);
#endif

    gl_FragColor = vec4(tonemap(sky), 1.0);
#else
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
#endif
}
