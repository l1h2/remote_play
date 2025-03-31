import subprocess

from utils import Paths, check_path


def run_app() -> None:
    check_path(Paths.app_executable, "Please run the packaging script first.")

    print(f"Running the {Paths.app_name} executable...")
    subprocess.Popen([Paths.app_executable])


if __name__ == "__main__":
    run_app()
