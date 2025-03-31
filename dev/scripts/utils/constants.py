import os
import platform


class Paths:
    root_dir = os.path.abspath(
        os.path.join(os.path.dirname(__file__), "..", "..", "..")
    )
    core_dir = os.path.join(root_dir, "core")
    gui_dir = os.path.join(root_dir, "gui")
    gui_bin_dir = os.path.join(gui_dir, "bin")
    gui_lib_dir = os.path.join(gui_dir, "lib")
    dev_dir = os.path.join(root_dir, "dev")
    cmake_build_dir = os.path.join(dev_dir, "cmake_build")
    cmake_bin_dir = os.path.join(cmake_build_dir, "bin")
    cmake_lib_dir = os.path.join(cmake_build_dir, "lib")
    pyinstaller_build_dir = os.path.join(dev_dir, "pyinstaller_build")
    pyinstaller_work_dir = os.path.join(pyinstaller_build_dir, "build")
    pyinstaller_dist_dir = os.path.join(pyinstaller_build_dir, "dist")
    pyinstaller_spec = os.path.join(pyinstaller_build_dir, "RemotePlay.spec")
    dist_dir = os.path.join(dev_dir, "dist")
    dist_bin_dir = os.path.join(dist_dir, "bin")
    dist_lib_dir = os.path.join(dist_dir, "lib")
    app_name = "RemotePlay"
    app_executable = os.path.join(
        dist_dir,
        app_name,
        f"{app_name}.exe" if platform.system() == "Windows" else app_name,
    )


class Flags:
    no_confirm = "-y"
