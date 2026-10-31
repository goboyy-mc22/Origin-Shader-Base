"""`pack` command: compile materials, write manifest.json and create a .mcpack."""

import json
import os
import re
import shutil
import sys
import uuid
import zipfile

from rich.console import Console

from build import compile_materials
from util import (
    ASSETS_DIR,
    BUILD_DIR,
    PACK_CONFIG_FILE,
    check_setup,
    get_available_materials,
    get_materials_path,
)

console = Console()

# Loader needed to use the pack on each platform (shown in the pack description)
LOADER_NOTICE = {
    "android": "Only works with MB Loader",
    "ios": "Only works with Minecraft with Hynis",
    "windows": "Only works with Wyvern Loader",
    "multiplatform": "Only works with MB Loader (Android), Wyvern Loader (Windows) "
                     "or Minecraft with Hynis (iOS)",
}

REQUIRED_KEYS = ("name", "version", "min_supported_mc_version", "uuid", "module_uuid", "materials")


def load_pack_config(patch_version: str = "") -> dict:
    try:
        import tomllib
    except ImportError:
        console.print("Error: Python 3.11 or newer is required (tomllib).", style="bold red")
        raise SystemExit(1)

    try:
        with open(PACK_CONFIG_FILE, "rb") as f:
            config = tomllib.load(f)
    except (OSError, tomllib.TOMLDecodeError) as e:
        console.print(f"Error in pack_config.toml: {e}", style="bold red", markup=False)
        raise SystemExit(1)

    for key in REQUIRED_KEYS:
        if key not in config:
            console.print(f"Error: pack_config.toml is missing '{key}'.", style="bold red")
            raise SystemExit(1)

    config.setdefault("description", "")
    config.setdefault("authors", [])
    config.setdefault("url", "")

    if patch_version:
        if not patch_version.isdigit():
            console.print(f"Error: invalid version '{patch_version}' (expected a number).", style="bold red")
            raise SystemExit(1)
        config["version"][2] = int(patch_version)

    available = get_available_materials()
    for name in config["materials"]:
        if name not in available:
            console.print(f"Error: material '{name}' from pack_config.toml does not exist.", style="bold red")
            raise SystemExit(1)
    return config


def create_manifest(config: dict, profile: str) -> dict:
    version = ".".join(str(v) for v in config["version"])
    description = (
        config["description"]
        .replace("%w", LOADER_NOTICE[profile])
        .replace("%v", f"v{version}-{profile}")
    )
    manifest = {
        "format_version": 2,
        "header": {
            "name": config["name"],
            "description": description,
            "uuid": config["uuid"],
            "version": config["version"],
            "min_engine_version": config["min_supported_mc_version"],
        },
        "modules": [
            {
                "type": "resources",
                "uuid": config["module_uuid"],
                "version": config["version"],
            }
        ],
        "metadata": {
            "authors": config["authors"],
            "url": config["url"],
        },
    }
    return manifest


def create_archive(pack_dir: str, archive_path: str) -> None:
    """Zip pack_dir with manifest.json at the archive root."""
    if os.path.exists(archive_path):
        os.remove(archive_path)
    with zipfile.ZipFile(archive_path, "w", zipfile.ZIP_DEFLATED) as archive:
        for folder, dirs, files in os.walk(pack_dir):
            dirs.sort()
            for name in sorted(files):
                full_path = os.path.join(folder, name)
                archive.write(full_path, os.path.relpath(full_path, pack_dir).replace(os.sep, "/"))


def run(args) -> None:
    if not check_setup(console):
        raise SystemExit(1)

    config = load_pack_config(args.v)
    profile = args.p
    version = ".".join(str(v) for v in config["version"])
    slug = config["name"].lower().replace(" ", "-")

    console.print("[bold green]Origin Pack Builder[/]")
    console.print(f"  [dim]name    :[/] [cyan]{config['name']}[/]")
    console.print(f"  [dim]authors :[/] [cyan]{', '.join(config['authors'])}[/]")
    console.print(f"  [dim]version :[/] [cyan]{version}[/]")
    console.print(f"  [dim]profile :[/] [cyan]{profile}[/]\n")

    pack_dir = os.path.join(BUILD_DIR, f"pack-{profile}")
    if os.path.exists(pack_dir):
        shutil.rmtree(pack_dir)
    if os.path.isdir(ASSETS_DIR):
        shutil.copytree(ASSETS_DIR, pack_dir)
    else:
        os.makedirs(pack_dir)
    if not os.path.exists(os.path.join(pack_dir, "pack_icon.png")):
        console.print("Warning: assets/pack_icon.png not found, the pack will have no icon.", style="yellow")

    manifest = create_manifest(config, profile)

    console.print("~ Build materials", style="bold")
    default_materials = get_materials_path(config["materials"])
    mats_dir = os.path.join(pack_dir, "renderer", "materials")
    if not compile_materials(profile, default_materials, mats_dir, strict=args.strict):
        raise SystemExit(1)
    if not any(f.endswith(".material.bin") for f in os.listdir(mats_dir)):
        console.print("Error: no .material.bin files were produced.", style="bold red")
        raise SystemExit(1)

    with open(os.path.join(pack_dir, "manifest.json"), "w", encoding="utf-8") as f:
        json.dump(manifest, f, indent=2, ensure_ascii=False)
    console.print("\n~ manifest.json written", style="bold")

    if args.no_zip:
        console.print(f"[bold green]Pack ready[/] -> {os.path.relpath(pack_dir)}")
        return

    archive_path = os.path.join(BUILD_DIR, f"{slug}-{version}-{profile}.mcpack")
    create_archive(pack_dir, archive_path)
    console.print(f"[bold green]Pack ready[/] -> {os.path.relpath(archive_path)}")


def run_manifest(args) -> None:
    """`manifest` command: print or write the manifest.json for a profile, or renew the pack UUIDs."""
    if args.new_uuids:
        renew_uuids()
        return

    config = load_pack_config(args.v)
    text = json.dumps(create_manifest(config, args.p), indent=2, ensure_ascii=False)
    if args.o:
        os.makedirs(os.path.dirname(os.path.abspath(args.o)), exist_ok=True)
        with open(args.o, "w", encoding="utf-8") as f:
            f.write(text + "\n")
        console.print(f"manifest written -> {args.o}", markup=False)
    else:
        print(text)


def renew_uuids() -> None:
    """Replace the pack and module UUIDs in pack_config.toml with new random ones."""
    with open(PACK_CONFIG_FILE, "r", encoding="utf-8") as f:
        text = f.read()
    for key in ("uuid", "module_uuid"):
        text, count = re.subn(rf'^{key}\s*=\s*"[^"]*"', f'{key} = "{uuid.uuid4()}"', text, flags=re.M)
        if count != 1:
            console.print(f"Error: could not find '{key}' in pack_config.toml.", style="bold red")
            raise SystemExit(1)
    with open(PACK_CONFIG_FILE, "w", encoding="utf-8", newline="") as f:
        f.write(text)
    console.print("New UUIDs written to source/Origin/pack_config.toml", markup=False)


if __name__ == "__main__":
    sys.exit("Run through the build tool: python tools pack -p <profile>")
