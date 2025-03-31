import sys

from builds import cmake, pyinstaller
from run import run_app

from utils import Flags, Paths, check_path, choice, copy_dir


def run_cmake(no_confirm: bool) -> None:
    prompt = "Do you want to run the CMake build script?"
    if choice(prompt, no_confirm):
        cmake()
    else:
        print("Skipping CMake build script.")


def run_pyinstaller(no_confirm: bool) -> None:
    prompt = "Do you want to run the PyInstaller build script?"
    if choice(prompt, no_confirm):
        pyinstaller()
    else:
        print("Skipping PyInstaller build script.")


def copy_dist() -> None:
    check_path(
        Paths.pyinstaller_dist_dir, "Please run the PyInstaller build script first."
    )
    copy_dir(Paths.pyinstaller_dist_dir, Paths.dist_dir)
    print("Packaging completed successfully.")


def run_exe(no_confirm: bool) -> None:
    prompt = "Do you want to run the executable?"
    if choice(prompt, no_confirm):
        run_app()
    else:
        print("Closing the script.")


def package_app() -> None:
    no_confirm = Flags.no_confirm in sys.argv

    run_cmake(no_confirm)
    run_pyinstaller(no_confirm)
    copy_dist()

    run_exe(no_confirm)


if __name__ == "__main__":
    package_app()
