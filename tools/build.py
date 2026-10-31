"""`mats` command and the shared compile routine used by `pack`."""

import os
from importlib import import_module

from rich.console import Console

from util import (
    BUILD_DIR,
    PROJECT_DIR,
    SHADERC_PATH,
    check_setup,
    get_materials_path,
    print_styled_error,
)

console = Console()
lp = import_module("lazurite.project.project")


def compile_materials(profile: str, materials: list, output_path: str,
                      defines: list = None, strict: bool = False) -> bool:
    """Compile materials for a profile into output_path.

    materials: list of material paths (empty list builds every material in the project).
    strict:    treat compiler warnings as errors.
    Returns True on success.
    """
    warnings = []

    def log(message):
        message = str(message)
        if message.startswith("Warning"):
            warnings.append(message)
        console.print(f"  {message}", markup=False, highlight=False)

    # lazurite reports progress through the module-level print()
    lp.print = log
    os.makedirs(output_path, exist_ok=True)

    try:
        lp.compile(
            project_path=PROJECT_DIR,
            profiles=[profile],
            output_folder=output_path,
            material_patterns=materials,
            shaderc_path=SHADERC_PATH,
            defines=defines or [],
        )
    except Exception as e:
        print_styled_error(console, e.args[0] if e.args else repr(e))
        return False

    if strict and warnings:
        console.print(f"Error: {len(warnings)} warning(s) found (strict mode).", style="bold red")
        return False
    return True


def run(args) -> None:
    if not check_setup(console):
        raise SystemExit(1)

    try:
        materials = get_materials_path(args.m)
    except FileNotFoundError as e:
        console.print(f"Error: material '{e.args[0]}' does not exist in the project.", style="bold red")
        raise SystemExit(1)

    output_path = os.path.join(BUILD_DIR, args.p)

    console.print(f"[bold green]Origin Material Builder[/] - profile: [cyan]{args.p}[/]")
    defines = []
    if args.s:
        from lazurite.compiler.macro_define import MacroDefine
        defines = [MacroDefine.from_string(args.s)]

    if not compile_materials(args.p, materials, output_path, defines, strict=args.strict):
        console.print("\nBuild failed.", style="bold red")
        raise SystemExit(1)
    console.print(f"\n[bold green]Build succeeded[/] -> {os.path.relpath(output_path)}")
