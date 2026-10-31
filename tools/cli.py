"""Command line interface of the Origin build tools."""

import argparse
import os

import build
import pack
import setup
from util import PROFILES


def main() -> None:
    parser = argparse.ArgumentParser(
        prog="build",
        description="Origin Shader build tool: wrapper around Lazurite to build the shader for RenderDragon",
        epilog="GitHub: https://github.com/goboyy-mc22/Origin-Shader-Base",
    )
    default_profile = "windows" if os.name == "nt" else "android"
    subparsers = parser.add_subparsers(dest="subcommand", help="sub-command")

    pack_parser = subparsers.add_parser("pack", help="build the shader pack (.mcpack)")
    pack_parser.set_defaults(func=pack.run)
    manifest_parser = subparsers.add_parser("manifest", help="print or write manifest.json, or renew pack UUIDs")
    manifest_parser.set_defaults(func=pack.run_manifest)
    mats_parser = subparsers.add_parser("mats", help="compile materials only")
    mats_parser.set_defaults(func=build.run)
    setup_parser = subparsers.add_parser("setup", help="download shaderc and base materials")
    setup_parser.set_defaults(func=setup.run)

    for sub in (pack_parser, mats_parser):
        sub.add_argument("-p", choices=PROFILES, default=default_profile, help="build profile")
        sub.add_argument("--strict", action="store_true", help="treat compiler warnings as errors")

    pack_parser.add_argument("-v", default="", help="patch version number, e.g. 5 builds x.y.5")
    pack_parser.add_argument("--no-zip", action="store_true", help="do not create the .mcpack archive")

    manifest_parser.add_argument("-p", choices=PROFILES, default=default_profile, help="build profile")
    manifest_parser.add_argument("-v", default="", help="patch version number, e.g. 5 builds x.y.5")
    manifest_parser.add_argument("-o", default="", help="write the manifest to this file instead of printing it")
    manifest_parser.add_argument("--new-uuids", action="store_true",
                                 help="generate new pack and module UUIDs in pack_config.toml")

    mats_parser.add_argument("-m", default=[], nargs="+", help="materials to build, e.g. Sky RenderChunk")
    mats_parser.add_argument("-s", default=[], nargs="+",
                             help="macros to define, e.g. ORIGIN_ENABLE_WATER_WAVES ORIGIN_ENABLE_LEAF_SWAY")

    setup_parser.add_argument("--reset", action="store_true", help="delete all downloaded data first")
    setup_parser.add_argument("--materials-version", default="",
                              help="serialized-materials release tag, e.g. v26.60")

    args = parser.parse_args()
    if args.subcommand is None:
        parser.print_help()
        raise SystemExit(1)
    args.func(args)
