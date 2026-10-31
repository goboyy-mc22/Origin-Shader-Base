"""Shared constants and helpers for the Origin build tools."""

import json
import os
import platform

from rich.console import Console

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

TOOLS_DIR = os.path.join(ROOT_DIR, "tools")
DATA_DIR = os.path.join(TOOLS_DIR, "data")
BASE_MATERIALS_DIR = os.path.join(DATA_DIR, "materials")
CONF_FILE = os.path.join(DATA_DIR, "setup.json")

PROJECT_DIR = os.path.join(ROOT_DIR, "source", "materials")
PACK_CONFIG_FILE = os.path.join(ROOT_DIR, "source", "Origin", "pack_config.toml")
ASSETS_DIR = os.path.join(ROOT_DIR, "assets")
BUILD_DIR = os.path.join(ROOT_DIR, "build")

PROFILES = ["android", "ios", "windows", "multiplatform"]

# External build dependencies (downloaded by `setup`)
SHADERC_URL_PREFIX = "https://github.com/devendrn/newb-shader/releases/download/dev/shaderc-"
SERIALIZED_MATERIALS_REPO = "mcpebd/serialized-materials"
SERIALIZED_MATERIALS_VERSION = "v26.60"

SHADERC_PATH = os.path.join(DATA_DIR, "shaderc.exe" if os.name == "nt" else "shaderc")


def get_available_materials() -> list:
    """Names of all material folders in the project."""
    if not os.path.isdir(PROJECT_DIR):
        return []
    return [
        d for d in sorted(os.listdir(PROJECT_DIR))
        if os.path.isdir(os.path.join(PROJECT_DIR, d)) and not d.startswith((".", "_"))
    ]


def get_materials_path(materials: list) -> list:
    """Resolve material names to project paths. Raises FileNotFoundError for unknown names."""
    available = get_available_materials()
    paths = []
    for name in materials:
        if name not in available:
            raise FileNotFoundError(name)
        paths.append(os.path.join(PROJECT_DIR, name))
    return paths


def load_conf() -> dict:
    try:
        with open(CONF_FILE, "r", encoding="utf-8") as f:
            conf = json.load(f)
        return conf if isinstance(conf, dict) else {}
    except (OSError, ValueError):
        return {}


def save_conf(conf: dict) -> None:
    os.makedirs(DATA_DIR, exist_ok=True)
    with open(CONF_FILE, "w", encoding="utf-8") as f:
        json.dump(conf, f, indent=2)


def check_setup(console: Console) -> bool:
    """Return True if the build tools are set up for this machine."""
    conf = load_conf()
    problems = []

    if conf.get("os_name") != platform.system() or conf.get("arch") != platform.machine():
        problems.append("setup was done on a different machine")
    if not os.path.exists(SHADERC_PATH):
        problems.append("shaderc compiler is missing")
    missing = [
        m for m in get_available_materials()
        if not os.path.exists(os.path.join(BASE_MATERIALS_DIR, f"{m}.material.json"))
    ]
    if missing:
        problems.append("base materials are missing: " + ", ".join(missing))

    for problem in problems:
        console.print(f"Error: {problem}.", style="red")
    if problems:
        console.print("       Please run `setup` first.", style="bold red")
    return not problems


def print_styled_error(console: Console, log: str) -> None:
    for line in str(log).strip().split("\n"):
        words = line.split()
        if not words:
            continue
        style = "dim"
        if words[0] == "Warning:":
            style = "red"
        elif words[0] in ("Error:", ">>>", "cpp:"):
            style = "bold red"
        console.print(line, style=style, markup=False, highlight=False)
