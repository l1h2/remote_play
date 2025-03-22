import subprocess

from utils import PATHS, check_path


def run_app() -> None:
    check_path(PATHS.app_executable, "Please run the packaging script first.")

    print(f"Running the {PATHS.app_name} executable...")
    subprocess.Popen([PATHS.app_executable])


if __name__ == "__main__":
    run_app()
