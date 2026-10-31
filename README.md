# Origin Shader Base

A custom vanilla RenderDragon shader base for Minecraft Bedrock. It targets Android, iOS and Windows, and can also be built as a single multi-platform pack.

## Project layout

```
.github/workflows/   build.yml (build packs), build-check.yml (compile check)
assets/              pack_icon.png and other resource pack files
include/             bgfx and MinecraftRenderer.Materials headers
source/
  Origin/
    Configuration.h  shader options (fog strength, ...)
    functions/       reusable shader functions (Sky, Fog, Lighting, Common)
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

Reusable code lives in `source/Origin/functions/` and is shared by the materials:

```glsl
#include <Origin/functions/Fog.h>

diffuse.rgb = originApplyFog(diffuse.rgb, FogColor.rgb, v_fog.a);
```

Options such as `ORIGIN_FOG_STRENGTH` are set in `source/Origin/Configuration.h`. The defaults match vanilla rendering.

## GitHub Actions

- **Build**: open *Actions → Build → Run workflow*, choose Android, iOS, Windows, Multi-platform or All, and download the `.mcpack` from the run's artifacts. The optional *subversion* input sets the patch version. Enable *release* to also get a draft GitHub release with the `.mcpack` files attached as direct downloads.
- **Build Check**: runs on every pull request and push to `main`. It only checks that all materials compile for all platforms. It packages and uploads nothing.

Windows and Multi-platform builds run on a Windows runner because Direct3D shaders can only be compiled on Windows.

## License

See [LICENSE](LICENSE).
