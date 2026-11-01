# Origin Shader Base

A custom vanilla RenderDragon shader base for Minecraft Bedrock. It targets Android, iOS and Windows, and can also be built as a single multi-platform pack.

## Project layout

```
.github/workflows/   build.yml (build packs), build-check.yml (compile check)
assets/              pack_icon.png and other resource pack files
include/             bgfx and MinecraftRenderer.Materials headers
source/
  Origin/
    insert.sh        single include for materials
    configuration.h  important values: sky colors, fog range, wave strength
    function/        shader functions, one file per topic
    pack_config.toml pack name, version, UUID, material list and subpacks (same format as Newb)
  materials/         material sources and project.json (build profiles)
tools/               build tool (setup, mats, pack, manifest)
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
python tools manifest --new-uuids        # generate a new pack UUID (do this for your own fork)
```

`build.sh` (Linux, macOS, Termux) and `build.bat` (Windows) run `python tools` for you, for example `./build.sh pack -p android`.

| Command | Options |
|---------|---------|
| `setup` | `--reset` delete downloaded data first, `--materials-version <tag>` use another serialized-materials release |
| `mats`  | `-p <profile>`, `-m <materials...>`, `-s <define>`, `--strict` |
| `pack`  | `-p <profile>`, `-v <patch>`, `--no-zip`, `--strict` |
| `manifest` | `-p <profile>`, `-v <patch>`, `-o <file>` write to a file, `--new-uuids` renew the pack UUID |

`--strict` turns compiler warnings into errors. The finished pack is written to `build/` as `origin-shader-base-<version>-<profile>.mcpack`.

## Pack manifest and subpacks

`manifest.json` is generated for every pack from `source/Origin/pack_config.toml`, which uses the same format as Newb's: name, version, minimum Minecraft version, description (`%w` = loader notice, `%v` = version), authors, url, `uuid`, `materials`, an `[info]` table (copyright and credits, written to `COPYRIGHT.txt` and `CREDITS.txt`) and `[[subpack]]` entries. The resource module UUID is derived from `uuid`. The pack is zipped with the manifest at the archive root, which is what Minecraft expects of a `.mcpack`.

Each `[[subpack]]` recompiles its `materials` with `define` set as a macro. They are listed left to right in the game, and the right-most one (`DEFAULT`) is enabled by default.

| Subpack (define) | Materials | Effect |
|------------------|-----------|--------|
| `NO_WAVE` | RenderChunk | no plant sway and water waves |
| `NO_FOG` | RenderChunk | no terrain fog and god rays |
| `NO_WAVE_NO_FOG` | RenderChunk | both of the above |
| `SHOOTING_STAR` | Sky | shooting stars |
| `GALAXY_STARS` | Sky | dense stars and a milky way band |
| `END_NEBULA` | EndSky | nebula in The End sky |
| `END_PLANET` | EndSky | distant planet in The End sky |
| `DEFAULT` | none | default look |

## Shader functions

Reusable code lives in `source/Origin/function/`, one file per topic, and materials include all of it through a single header:

```glsl
#include <Origin/insert.sh>

diffuse.rgb = blendFog(diffuse.rgb, v_fog.rgb, v_fog.a);
```

Functions a material does not call are dropped by the compiler. Every function in the library is called by at least one material.

| File | Contents |
|------|----------|
| `common.h` | `PI`, `luminance`, `rotation2d` |
| `noise.h` | `hash31`, `noise3d` |
| `coloring.h` | `tonemap` |
| `detection.h` | `Environment`, rain, day, sunrise/sunset, underwater, Nether and End detection, sun and moon direction, water/tint/leaf vertex checks |
| `sky.h` | sky colors (day, night, sunrise/sunset, rain, underwater, The End), `renderSky`, `renderEndSky`, shooting star, galaxy stars, End nebula and planet, sun/moon and star colors |
| `fog.h` | `fogFade` (low/high range, rain, mist), `computeFog`, `blendFog`, `godRay` (fan rays at sunrise and sunset) |
| `wave.h` | `waveFade`, `windStrength`, `plantSway`, `waterWave` |
| `water.h` | `fresnel`, `shadeWater` |
| `glow.h` | `glowFromTexture` (glowing pixels marked in the texture alpha) |
| `lighting.h` | `shadeWithLightmap`, `blendOverlay` |
| `weather.h` | `weatherLight` |
| `clouds.h` | `cloudColor` |

Sky, EndSky and terrain fog use the same sky colors, so terrain blends into the sky at the horizon. The Sky material is drawn as a full screen quad and needs the `TimeOfDay` uniform, which is added with `source/materials/Sky/material.json`.

## Configuration

`source/Origin/configuration.h` only holds the important values:

| Group | Settings |
|-------|----------|
| Sky colors | `DAY_*`, `NIGHT_*`, `DAWN_*` (sunrise and sunset), `RAIN_*`, `END_*` with zenith, horizon and edge colors |
| Fog | `FOG_LOW`, `FOG_HIGH`, `RAIN_FOG_LOW`, `RAIN_FOG_HIGH`, `GODRAY_STRENGTH` |
| Waves | `WAVE_RANGE`, `PLANT_SWAY_STRENGTH`, `WATER_WAVE_HEIGHT` |

The sky, sun glow, God rays, plant sway and water waves have not been checked in game yet. Plant sway picks vertices from the tinted color and the upper half of the terrain atlas tile (assuming 32 tile rows), which can differ with resource packs.

## Pack assets

`assets/` is copied into every pack: `pack_icon.png`, plus `biomes_client.json`, `biomes/` and `fogs/` that give The End a fixed purple fog color (`#400040`), which `detectEnd` relies on. Block textures with glow are not included: to make a block glow, edit its texture so the glowing pixels have alpha 252/255 (full) or 253/255 (partial) and put it in `assets/textures/blocks/`.

## Build assets

`python tools setup` downloads the shaderc compilers and the serialized base materials from this repository's `build-assets` release, so builds do not depend on other repositories. To update them (for example for a new `serialized-materials` tag), run *Actions → Mirror build assets* with the tag, then change `SERIALIZED_MATERIALS_VERSION` in `tools/util.py`.

## GitHub Actions

- **Build**: open *Actions → Build → Run workflow*, choose Android, iOS, Windows, Multi-platform or All, and download the `.mcpack` from the run's artifacts. The optional *subversion* input sets the patch version. Enable *release* to also get a draft GitHub release with the `.mcpack` files attached as direct downloads.
- **Build Check**: runs on every pull request and push to `main`. It builds the multi-platform pack with all subpacks without archiving, so it only checks that everything compiles. It uploads nothing.

Windows and Multi-platform builds run on a Windows runner because Direct3D shaders can only be compiled on Windows.

## License

See [LICENSE](LICENSE).
