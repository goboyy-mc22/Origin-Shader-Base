#!/usr/bin/env python3
"""
Pack builder: compile materials -> build/pack-<platform>/ -> manifest.json -> .mcpack

Usage (via build.py):
  python tools/build.py pack -p android
  python tools/build.py pack -p multiplatform -v 5 --no-zip
"""

import argparse
import json
import os
import shutil
import sys
import zipfile

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD_DIR = os.path.join(ROOT_DIR, "build")
ASSETS_DIR = os.path.join(ROOT_DIR, "assets")
CONFIG_PATH = os.path.join(ROOT_DIR, "pack_config.json")

PLATFORMS = ["android", "ios", "windows", "multiplatform"]

# Loader yang dibutuhkan tiap platform (ditampilkan di deskripsi pack, sama seperti Newb)
PATCH_WARNING = {
    "android": "Only works with MB Loader",
    "ios": "Only works with Minecraft with Hynis",
    "windows": "Only works with Wyvern Loader",
    "multiplatform": "Only works with MB Loader (Android) or Wyvern Loader (Windows) or Minecraft with Hynis (iOS)",
}


def load_config(patch=None):
    with open(CONFIG_PATH, "r", encoding="utf-8") as f:
        cfg = json.load(f)
    for key in ("name", "slug", "version", "min_engine_version", "uuid_header", "uuid_module"):
        if key not in cfg:
            raise SystemExit(f"pack_config.json: field '{key}' is missing")
    if patch is not None:
        cfg["version"][2] = int(patch)
    return cfg


def create_manifest(cfg, platform):
    version_str = ".".join(str(v) for v in cfg["version"])
    description = (
        cfg.get("description", "")
        .replace("%w", PATCH_WARNING[platform])
        .replace("%v", f"v{version_str}-{platform}")
    )
    return {
        "format_version": 2,
        "header": {
            "name": cfg["name"],
            "description": description,
            "uuid": cfg["uuid_header"],
            "version": cfg["version"],
            "min_engine_version": cfg["min_engine_version"],
        },
        "modules": [
            {
                "type": "resources",
                "uuid": cfg["uuid_module"],
                "version": cfg["version"],
            }
        ],
        "metadata": {
            "authors": cfg.get("authors", []),
            "url": cfg.get("url", ""),
        },
    }


def make_mcpack(pack_dir, out_file):
    """Zip isi pack_dir (manifest.json harus di root zip) lalu simpan sebagai .mcpack."""
    os.makedirs(os.path.dirname(out_file), exist_ok=True)
    if os.path.exists(out_file):
        os.remove(out_file)
    with zipfile.ZipFile(out_file, "w", zipfile.ZIP_DEFLATED) as zf:
        for folder, dirs, files in os.walk(pack_dir):
            dirs.sort()
            for name in sorted(files):
                full = os.path.join(folder, name)
                arc = os.path.relpath(full, pack_dir).replace(os.sep, "/")
                zf.write(full, arc)


def build_pack(platform, build_materials, patch=None, make_zip=True):
    """build_materials(platform, material_name, out_dir) -> bool, disediakan oleh build.py"""
    cfg = load_config(patch)
    version_str = ".".join(str(v) for v in cfg["version"])

    pack_dir = os.path.join(BUILD_DIR, f"pack-{platform}")
    mats_dir = os.path.join(pack_dir, "renderer", "materials")

    if os.path.exists(pack_dir):
        shutil.rmtree(pack_dir)

    # 1. salin assets (pack_icon.png, texture, dll)
    if os.path.isdir(ASSETS_DIR):
        shutil.copytree(ASSETS_DIR, pack_dir)
    else:
        os.makedirs(pack_dir)
    if not os.path.exists(os.path.join(pack_dir, "pack_icon.png")):
        print(" Warning: assets/pack_icon.png not found (pack will have no icon)")

    # 2. compile semua material langsung ke renderer/materials
    os.makedirs(mats_dir, exist_ok=True)
    if not build_materials(platform, None, mats_dir):
        return False
    if not any(n.endswith(".material.bin") for n in os.listdir(mats_dir)):
        print(" Error: no .material.bin produced, refusing to pack an empty shader")
        return False

    # 3. manifest.json
    manifest = create_manifest(cfg, platform)
    with open(os.path.join(pack_dir, "manifest.json"), "w", encoding="utf-8") as f:
        json.dump(manifest, f, indent=2, ensure_ascii=False)
    print(f" manifest.json written ({cfg['name']} v{version_str})")

    # 4. .mcpack
    if make_zip:
        out_file = os.path.join(BUILD_DIR, f"{cfg['slug']}-{version_str}-{platform}.mcpack")
        make_mcpack(pack_dir, out_file)
        print(f" Packed -> {os.path.relpath(out_file, ROOT_DIR)}")
    return True


def main(argv, build_materials):
    parser = argparse.ArgumentParser(prog="build.py pack", description="Build shader pack (.mcpack)")
    parser.add_argument("-p", choices=PLATFORMS, default="windows" if os.name == "nt" else "android",
                        help="target platform")
    parser.add_argument("-v", default="", help="patch version number, e.g. 5 -> x.y.5")
    parser.add_argument("--no-zip", action="store_true", help="don't create .mcpack")
    args = parser.parse_args(argv)
    if args.v and not args.v.isdigit():
        print(f" Invalid version '{args.v}', must be a number")
        return False
    return build_pack(args.p, build_materials, patch=args.v or None, make_zip=not args.no_zip)


if __name__ == "__main__":
    sys.exit("Run via: python tools/build.py pack -p <platform>")
