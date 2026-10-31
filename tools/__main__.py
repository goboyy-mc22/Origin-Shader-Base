"""Entry point: python tools <command>"""

import os
import sys
from importlib.metadata import PackageNotFoundError


def _check_requirements() -> None:
    from importlib import metadata
    from packaging.version import parse

    requirements = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "requirements.txt")
    with open(requirements, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip().replace(" ", "")
            if "==" in line:
                name, wanted = line.split("==")
                installed = metadata.version(name)
                if parse(installed) != parse(wanted):
                    print(f'Warning: using "{name}" {installed} instead of {wanted}')
            elif ">=" in line:
                name, wanted = line.split(">=")
                installed = metadata.version(name)
                if parse(installed) < parse(wanted):
                    print(f'Warning: using "{name}" {installed}, {wanted} or newer is expected')


if __name__ == "__main__":
    try:
        _check_requirements()
        from cli import main

        main()
    except (ModuleNotFoundError, PackageNotFoundError) as e:
        print(f'Module "{getattr(e, "name", None) or e}" not found.')
        print("\nDid you run `python -m pip install -r requirements.txt`?\n")
        sys.exit(1)
    except KeyboardInterrupt:
        sys.exit(1)
