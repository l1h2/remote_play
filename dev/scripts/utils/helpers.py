import os
import shutil
import subprocess
import sys


def check_path(path: str, instructions: str) -> None:
    if not os.path.exists(path):
        print(f"Error: {path} not found. {instructions}", file=sys.stderr)
        sys.exit(1)


def run_command(command: str) -> None:
    result = subprocess.run(command, shell=True)
    if result.returncode != 0:
        print(f"Error: Failed to run command: {command}", file=sys.stderr)
        sys.exit(1)


def choice(prompt: str, no_confirm: bool = False) -> bool:
    if no_confirm:
        return True

    confirm = input(f"{prompt} (Y/n): ")
    return confirm.lower() in ["", "y"]


def clean_dir(dir: str) -> None:
    if os.path.exists(dir):
        shutil.rmtree(dir)
    os.makedirs(dir)


def copy_dir(src_dir: str, dst_dir: str) -> None:
    print(f"Copying files from {src_dir} to {dst_dir}")

    clean_dir(dst_dir)
    shutil.copytree(src_dir, dst_dir, dirs_exist_ok=True)

    print("Files copied successfully.")
