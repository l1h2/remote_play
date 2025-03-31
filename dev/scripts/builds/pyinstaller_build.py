import os
import sys

if __name__ == "__main__":
    sys.path.append(os.path.join(os.path.dirname(__file__), ".."))


from utils import Paths, check_path, run_command


def pyinstaller() -> None:
    error_instruction = "Please run the PyInstaller setup script first."
    check_path(Paths.pyinstaller_build_dir, error_instruction)
    check_path(Paths.pyinstaller_spec, error_instruction)

    run_command(
        f"pyinstaller "
        f"--clean "
        f"--noconfirm "
        f'--workpath "{Paths.pyinstaller_work_dir}" '
        f'--distpath "{Paths.pyinstaller_dist_dir}" '
        f'"{Paths.pyinstaller_spec}"'
    )


if __name__ == "__main__":
    pyinstaller()
