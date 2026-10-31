# Origin Shader Base

A custom vanilla RenderDragon shader base for Minecraft Bedrock. It targets Android, iOS and Windows, and can also be built as a single multi-platform pack.

## Project layout

```
.github/workflows/   build.yml (build packs), build-check.yml (compile check)
assets/              pack_icon.png and other resource pack files
include/             bgfx and MinecraftRenderer.Materials headers
source/
  Origin/
    Origin.h         single include for materials
    Configuration.h  shader options (fog strength, ...)
    functions/       shader functions, one file per topic
    pack_config.toml pack name, version, UUIDs and material list
  materials/         material sources and project.json (build profiles)
tools/               build tool (setup, mats, pack)
build.sh, build.bat  launchers for the build tool
```

## Build profiles

| Profile         | Shader format(s)                  | Needs a loader                                   |
|-----------------|-----------------------------------|--------------------------------------------------|
| `android`       | ESSL 3.10                         | MB Loader                                        |
| `ios`           | Metal                             | Minecraft with Hynis                             |
| `windows`       | Direct3D SM 5.0                   | Wyvern Loader                                    |
| `multiplatform` | all of the above in one pack      | any of the loaders above                         |

Shaders are not officially supported by Minecraft Bedrock. The loaders are unofficial ways to load them.

## Building locally

Requirements: Python 3.11 or newer.

```
pip install -r requirements.txt
python tools setup                       # download shaderc and base materials (once)
python tools pack -p android             # android | ios | windows | multiplatform
python tools pack -p multiplatform -v 5  # patch version 5 -> v0.1.5
python tools mats -p windows -m Sky      # compile selected materials only
python tools manifest -p android         # print the generated manifest.json
python tools manifest --new-uuids        # generate new pack UUIDs (do this for your own fork)
```

`build.sh` (Linux, macOS, Termux) and `build.bat` (Windows) run `python tools` for you, for example `./build.sh pack -p android`.

| Command | Options |
|---------|---------|
| `setup` | `--reset` delete downloaded data first, `--materials-version <tag>` use another serialized-materials release |
| `mats`  | `-p <profile>`, `-m <materials...>`, `-s <define>`, `--strict` |
| `pack`  | `-p <profile>`, `-v <patch>`, `--no-zip`, `--strict` |
| `manifest` | `-p <profile>`, `-v <patch>`, `-o <file>` write to a file, `--new-uuids` renew the UUIDs |

`--strict` turns compiler warnings into errors. The finished pack is written to `build/` as `origin-shader-base-<version>-<profile>.mcpack`.

## Pack manifest

`manifest.json` is generated for every pack from `source/Origin/pack_config.toml` (name, description, version, minimum Minecraft version, UUIDs, authors and the material list). The pack is then zipped with the manifest at the archive root, which is what Minecraft expects of a `.mcpack`.

## Shader functions

Reusable code lives in `source/Origin/functions/`, one file per topic. Materials include everything through a single header:

```glsl
#include <Origin/Origin.h>

diffuse.rgb = originApplyFog(diffuse.rgb, FogColor.rgb, v_fog.a);
```

Unused functions are removed by the shader compiler, so including everything costs nothing. Options such as `ORIGIN_FOG_STRENGTH` are set in `source/Origin/Configuration.h`, and the defaults match vanilla rendering.

| File | Contents |
|------|----------|
| `Common.h` | constants, `originLuma`, `originSq`, `originRemap` |
| `Noise.h` | `originHash`, `originNoise`, `originFbm` |
| `Color.h` | gamma conversion, saturation, contrast, tonemapping, `originGrade` |
| `Sky.h` | three-band sky color (zenith, horizon, edge), horizon and sun glow, sun and moon, stars |
| `Clouds.h` | cloud color, procedural density, shading |
| `Fog.h` | fog blending, linear and exponential fog |
| `Lighting.h` | light map, ambient occlusion, point light, entity overlay, enchantment glint |
| `Water.h` | fresnel, depth tint, reflection |
| `Wave.h` | water waves, leaf sway and the checks that pick which vertices move |
| `Weather.h` | rain and snow color and light |

Every function is compiled into the materials. Sky, fog, lighting, sun and moon, stars, clouds, weather and entity overlay are applied already. Water waves and leaf sway are wired into `RenderChunk` but switched off by default. The color grading and water shading functions are not applied yet. With the defaults, rendering is vanilla.

## Configuration

All settings live in `source/Origin/Configuration.h`:

| Group | Settings |
|-------|----------|
| Sky | `ORIGIN_SKY_ZENITH_TINT`, `ORIGIN_SKY_HORIZON_TINT`, `ORIGIN_SKY_EDGE_TINT`, `ORIGIN_SKY_HORIZON_POSITION`, sun/moon and star brightness |
| Fog | `ORIGIN_FOG_STRENGTH`, `ORIGIN_FOG_TINT` |
| Clouds | `ORIGIN_CLOUD_TINT`, `ORIGIN_CLOUD_OPACITY` |
| Lighting | `ORIGIN_LIGHT_TINT` |
| Weather | `ORIGIN_RAIN_OPACITY` |
| Water waves | `ORIGIN_ENABLE_WATER_WAVES`, `ORIGIN_WATER_WAVE_HEIGHT`, `ORIGIN_WATER_WAVE_SPEED`, `ORIGIN_WATER_WAVE_SCALE` |
| Leaf sway | `ORIGIN_ENABLE_LEAF_SWAY`, `ORIGIN_LEAF_SWAY_STRENGTH`, `ORIGIN_LEAF_SWAY_SPEED` |
| Distance | `ORIGIN_WAVE_RANGE` (waves and sway fade out at this distance) |
| Grading | `ORIGIN_EXPOSURE`, `ORIGIN_SATURATION`, `ORIGIN_CONTRAST` |

Colors are tints applied on top of the colors the game provides, so `vec3(1.0, 1.0, 1.0)` means unchanged. To enable water waves or leaf sway, remove the `//` in front of the matching `#define`. You can also try them without editing the file: `python tools mats -p android -s ORIGIN_ENABLE_WATER_WAVES ORIGIN_ENABLE_LEAF_SWAY`.

Leaf sway moves vertices of full tinted cubes in the alpha-tested pass (tree leaves). Leaves without a biome tint do not sway. Water waves move vertices of translucent water. Both rely on checks in `Wave.h` that are not tested in game yet.

## GitHub Actions

- **Build**: open *Actions → Build → Run workflow*, choose Android, iOS, Windows, Multi-platform or All, and download the `.mcpack` from the run's artifacts. The optional *subversion* input sets the patch version. Enable *release* to also get a draft GitHub release with the `.mcpack` files attached as direct downloads.
- **Build Check**: runs on every pull request and push to `main`. It only checks that all materials compile for all platforms. It packages and uploads nothing.

Windows and Multi-platform builds run on a Windows runner because Direct3D shaders can only be compiled on Windows.

## License

See [LICENSE](LICENSE).
