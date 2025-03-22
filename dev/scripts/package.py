import sys

from builds import cmake, pyinstaller
from utils import FLAGS, PATHS, check_path, choice, copy_dir


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
        PATHS.pyinstaller_dist_dir, "Please run the PyInstaller build script first."
    )
    copy_dir(PATHS.pyinstaller_dist_dir, PATHS.dist_dir)
    print("Packaging completed successfully.")


def run_exe(no_confirm: bool) -> None:
    prompt = "Do you want to run the executable?"
    if choice(prompt, no_confirm):
        pass
    else:
        print("Closing the script.")


def package_app() -> None:
    no_confirm = FLAGS.no_confirm in sys.argv

    run_cmake(no_confirm)
    run_pyinstaller(no_confirm)
    copy_dist()

    run_exe(no_confirm)


if __name__ == "__main__":
    package_app()
