$input v_posTime, v_texcoord0

#include <bgfx_shader.sh>
#include <Origin/insert.sh>

SAMPLER2D_AUTOREG(s_SkyTexture);

void main() {
    vec3 dir = normalize(v_posTime.xyz);
    float t = v_posTime.w;

    vec3 sky = renderEndSky(dir, t);

#ifdef END_NEBULA
    sky += endNebula(dir, t);
#endif
#ifdef END_PLANET
    sky += endPlanet(dir);
#endif

    // Stars come from the vanilla sky texture
    sky += 2.8 * texture2D(s_SkyTexture, v_texcoord0).rgb;

    gl_FragColor = vec4(tonemap(sky), 1.0);
}
