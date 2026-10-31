# Origin-Shader-Base
A custom vanilla RenderDragon shader for Minecraft Bedrock

## Building

Requires Python 3.11+ and [lazurite](https://pypi.org/project/lazurite/).

```
pip install -r requirements.txt
python tools/build.py setup                    # download shaderc + base materials
python tools/build.py pack -p android          # android | ios | windows | multiplatform
python tools/build.py pack -p multiplatform -v 5 --no-zip
```

The `.mcpack` is written to `build/`. Compile a single material without packing:
`python tools/build.py RenderChunk android`.

On GitHub: **Actions → build → Run workflow**, pick the platform (Android, iOS, Windows, Multi-platform, All) and download the `.mcpack` from the artifacts.

Windows and Multi-platform builds run on a Windows runner because Direct3D shaders can only be compiled on Windows.
