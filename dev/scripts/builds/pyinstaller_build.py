import os
import sys

if __name__ == "__main__":
    sys.path.append(os.path.join(os.path.dirname(__file__), ".."))


from utils import PATHS, check_path, run_command


def pyinstaller() -> None:
    error_instruction = "Please run the PyInstaller setup script first."
    check_path(PATHS.pyinstaller_build_dir, error_instruction)
    check_path(PATHS.pyinstaller_spec, error_instruction)

    run_command(
        f"pyinstaller "
        f"--clean "
        f"--noconfirm "
        f'--workpath "{PATHS.pyinstaller_work_dir}" '
        f'--distpath "{PATHS.pyinstaller_dist_dir}" '
        f'"{PATHS.pyinstaller_spec}"'
    )


if __name__ == "__main__":
    pyinstaller()
