#!/bin/bash
# Origin Shader build tool launcher (Linux, macOS, Termux)
cd "$(dirname "$0")" || exit 1
command -v python3 >/dev/null 2>&1 || { echo "Python 3.11+ not found." >&2; exit 1; }
LD_LIBRARY_PATH=./tools/lib python3 tools "$@"
