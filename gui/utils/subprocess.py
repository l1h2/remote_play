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


@dataclass(frozen=True)
class InterprocessMessages:
    STREAM_REQUEST = "stream_request"
    ACK_STREAM_REQUEST = "ack_stream_request"
    STREAM_ACCEPT = "stream_accept"
    ACK_STREAM_ACCEPT = "ack_stream_accept"
    STREAM_REJECT = "stream_reject"
    ACK_STREAM_REJECT = "ack_stream_reject"
