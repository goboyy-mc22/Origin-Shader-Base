# Origin Shader Base

A custom vanilla RenderDragon shader base for Minecraft Bedrock. It targets Android, iOS and Windows, and can also be built as a single multi-platform pack.

## Project layout

```
.github/workflows/   build.yml (build packs), build-check.yml (compile check)
assets/              pack_icon.png and other resource pack files
include/             bgfx and MinecraftRenderer.Materials headers
source/
  Origin/            shader code and pack_config.toml
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
```

`build.sh` (Linux, macOS, Termux) and `build.bat` (Windows) run `python tools` for you, for example `./build.sh pack -p android`.

| Command | Options |
|---------|---------|
| `setup` | `--reset` delete downloaded data first, `--materials-version <tag>` use another serialized-materials release |
| `mats`  | `-p <profile>`, `-m <materials...>`, `-s <define>`, `--strict` |
| `pack`  | `-p <profile>`, `-v <patch>`, `--no-zip`, `--strict` |

`--strict` turns compiler warnings into errors. The finished pack is written to `build/` as `origin-shader-base-<version>-<profile>.mcpack`. Pack name, version, UUIDs and materials are configured in `source/Origin/pack_config.toml`.

## GitHub Actions

- **Build**: open *Actions → Build → Run workflow*, choose Android, iOS, Windows, Multi-platform or All, and download the `.mcpack` from the run's artifacts. The optional *subversion* input sets the patch version.
- **Build Check**: runs on every pull request and push to `main`. It only checks that all materials compile for all platforms. It packages and uploads nothing.

Windows and Multi-platform builds run on a Windows runner because Direct3D shaders can only be compiled on Windows.

## License

See [LICENSE](LICENSE).
