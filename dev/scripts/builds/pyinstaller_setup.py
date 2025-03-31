import os
import sys

if __name__ == "__main__":
    sys.path.append(os.path.join(os.path.dirname(__file__), ".."))


from utils import Paths, check_path, clean_dir, run_command


def setup() -> None:
    check_path(Paths.cmake_build_dir, "Please run the CMake build script first.")

    clean_dir(Paths.pyinstaller_build_dir)
    run_command(
        f"pyinstaller "
        f"--name {Paths.app_name} "
        f"--windowed "
        f'--workpath "{Paths.pyinstaller_work_dir}" '
        f'--distpath "{Paths.pyinstaller_dist_dir}" '
        f'--specpath "{Paths.pyinstaller_build_dir}" '
        f'--add-data "{Paths.cmake_bin_dir}":./bin '
        f'"{os.path.join(Paths.gui_dir, "main.py")}"'
    )


if __name__ == "__main__":
    setup()
