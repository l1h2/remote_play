import os
from dataclasses import dataclass


@dataclass(frozen=True)
class Defaults:
    LOCAL_PORT: int = 12345


@dataclass(frozen=True)
class Paths:
    ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    BIN = os.path.join(ROOT, "bin")
    LIB = os.path.join(ROOT, "lib")
