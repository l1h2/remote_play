import os
import sys

if __name__ == "__main__":
    sys.path.append(os.path.join(os.path.dirname(__file__), ".."))


from utils import PATHS, check_path, clean_dir, run_command


def setup() -> None:
    check_path(PATHS.cmake_build_dir, "Please run the CMake build script first.")

    clean_dir(PATHS.pyinstaller_build_dir)
    run_command(
        f"pyinstaller "
        f"--name {PATHS.app_name} "
        f"--windowed "
        f'--workpath "{PATHS.pyinstaller_work_dir}" '
        f'--distpath "{PATHS.pyinstaller_dist_dir}" '
        f'--specpath "{PATHS.pyinstaller_build_dir}" '
        f'--add-data "{PATHS.cmake_bin_dir}":./bin '
        f'"{os.path.join(PATHS.gui_dir, "main.py")}"'
    )


if __name__ == "__main__":
    setup()
