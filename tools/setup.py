"""`setup` command: download the shaderc compiler and the base materials."""

import os
import platform
import shutil
import zipfile
from urllib.request import Request, urlopen

from rich.progress import BarColumn, DownloadColumn, Progress, TextColumn, TransferSpeedColumn

from util import (
    BASE_MATERIALS_DIR,
    DATA_DIR,
    ASSETS_URL,
    SERIALIZED_MATERIALS_VERSION,
    SHADERC_PATH,
    SHADERC_URL_PREFIX,
    TOOLS_DIR,
    get_available_materials,
    load_conf,
    save_conf,
)

progress = Progress(
    TextColumn("[bold blue]{task.fields[filename]}", justify="right"),
    BarColumn(bar_width=None),
    "•",
    DownloadColumn(),
    "•",
    TransferSpeedColumn(),
)


def _download(url: str, path: str) -> None:
    filename = url.split("/")[-1]
    task_id = progress.add_task("download", filename=filename, start=False)
    request = Request(url, headers={"User-Agent": "Mozilla/5.0"})
    with urlopen(request, timeout=60) as response, open(path, "wb") as dest:
        length = response.info().get("Content-Length")
        progress.update(task_id, total=int(length) if length else None)
        progress.start_task(task_id)
        while True:
            chunk = response.read(32768)
            if not chunk:
                break
            dest.write(chunk)
            progress.update(task_id, advance=len(chunk))


def get_shaderc_url(os_name: str, arch: str):
    if os_name == "Windows":
        return SHADERC_URL_PREFIX + "win-x64.exe"
    if os_name == "Darwin":
        return SHADERC_URL_PREFIX + "osx-x64"
    if os_name in ("Linux", "Android"):
        if arch == "x86_64":
            return SHADERC_URL_PREFIX + "linux-x64"
        if arch == "aarch64":
            return SHADERC_URL_PREFIX + "android-arm64"
        if arch in ("armv8l", "armv7l"):
            return SHADERC_URL_PREFIX + "android-arm"
        progress.console.print(f"No shaderc build found for architecture '{arch}'", style="red")
        return None
    progress.console.print(f"Unsupported platform '{os_name}'", style="red")
    return None


def _apply_termux_fix() -> None:
    """libc++_shared.so is not found by default on Termux."""
    lib_dir = os.path.join(TOOLS_DIR, "lib")
    os.makedirs(lib_dir, exist_ok=True)
    termux_lib = "/data/data/com.termux/files/usr/lib/libc++_shared.so"
    target = os.path.join(lib_dir, "libc++_shared.so")
    if os.path.exists(termux_lib) and not os.path.exists(target):
        progress.console.print("Applying Termux fix for libc++_shared.so")
        shutil.copyfile(termux_lib, target)


def _download_base_materials(version: str) -> None:
    """Download serialized materials and keep only the ones this project builds."""
    url = f"{ASSETS_URL}materials-{version}.zip"
    zip_path = os.path.join(DATA_DIR, "materials.zip")
    shutil.rmtree(BASE_MATERIALS_DIR, ignore_errors=True)
    os.makedirs(BASE_MATERIALS_DIR)

    _download(url, zip_path)
    wanted = {f"{m.lower()}.material.json": m for m in get_available_materials()}
    extracted = set()
    try:
        with zipfile.ZipFile(zip_path) as archive:
            for member in archive.infolist():
                if member.is_dir():
                    continue
                name = wanted.get(os.path.basename(member.filename).lower())
                if name is None:
                    continue
                with archive.open(member) as src, open(
                    os.path.join(BASE_MATERIALS_DIR, f"{name}.material.json"), "wb"
                ) as dst:
                    shutil.copyfileobj(src, dst)
                extracted.add(name)
    finally:
        os.remove(zip_path)

    missing = sorted(set(wanted.values()) - extracted)
    if missing:
        raise RuntimeError(f"Base materials {version} are missing: {', '.join(missing)}")


def run(args) -> None:
    os_name = platform.system()
    arch = platform.machine()
    version = args.materials_version or SERIALIZED_MATERIALS_VERSION

    shaderc_url = get_shaderc_url(os_name, arch)
    if shaderc_url is None:
        raise SystemExit(1)

    if args.reset:
        shutil.rmtree(DATA_DIR, ignore_errors=True)
    os.makedirs(DATA_DIR, exist_ok=True)
    if os_name == "Android":
        _apply_termux_fix()

    conf = load_conf()
    if conf.get("shaderc_url") != shaderc_url and os.path.exists(SHADERC_PATH):
        os.remove(SHADERC_PATH)

    with progress:
        if not os.path.exists(SHADERC_PATH):
            progress.console.print("Downloading shaderc")
            _download(shaderc_url, SHADERC_PATH)
            os.chmod(SHADERC_PATH, 0o755)

        materials_ready = all(
            os.path.exists(os.path.join(BASE_MATERIALS_DIR, f"{m}.material.json"))
            for m in get_available_materials()
        )
        if conf.get("materials_version") != version or not materials_ready:
            progress.console.print(f"Downloading base materials {version}")
            _download_base_materials(version)

    save_conf({
        "os_name": os_name,
        "arch": arch,
        "shaderc_url": shaderc_url,
        "materials_version": version,
    })
    progress.console.print("[bold green]Setup complete!")
