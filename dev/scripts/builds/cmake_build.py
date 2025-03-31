import os
import sys

if __name__ == "__main__":
    sys.path.append(os.path.join(os.path.dirname(__file__), ".."))


from utils import Flags, Paths, choice, clean_dir, copy_dir, run_command


def cmake() -> None:
    no_confirm = Flags.no_confirm in sys.argv

    clean_dir(Paths.cmake_build_dir)
    os.chdir(Paths.cmake_build_dir)

    # Core dir in relation to the cmake_build directory
    # Using absolute path causes issues with CMake path resolution
    relative_core_dir = os.path.join("..", "..", "core")
    run_command(f"cmake {relative_core_dir}")
    run_command("cmake --build .")

    prompt = "Do you want to copy the compiled files to the GUI directory?"
    if not choice(prompt, no_confirm):
        print("Skipping copying compiled files to the GUI directory.")
        sys.exit(0)

    copy_dir(Paths.cmake_bin_dir, Paths.gui_bin_dir)


if __name__ == "__main__":
    cmake()
