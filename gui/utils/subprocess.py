import os
from dataclasses import dataclass

from .constants import Paths


def _get_process_name(name: str) -> str:
    return name + ".exe" if os.name == "nt" else name


@dataclass(frozen=True)
class _SubprocessNames:
    STUN_CLIENT: str = _get_process_name("stun_client")
    UDP_CONNECTION: str = _get_process_name("udp_connection")


@dataclass(frozen=True)
class Subprocess:
    STUN_CLIENT: str = os.path.join(Paths.BIN, _SubprocessNames.STUN_CLIENT)
    UDP_CONNECTION: str = os.path.join(Paths.BIN, _SubprocessNames.UDP_CONNECTION)
