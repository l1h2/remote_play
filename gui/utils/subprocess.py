import os
from enum import Enum

from .constants import Paths


def _get_process_name(name: str) -> str:
    return name + ".exe" if os.name == "nt" else name


class SubprocessNames(Enum):
    STUN_CLIENT = _get_process_name("stun_client")
    UDP_CONNECTION = _get_process_name("udp_connection")
    UDP_SERVER = _get_process_name("udp_server")
    UDP_CLIENT = _get_process_name("udp_client")


class Subprocess:
    STUN_CLIENT = os.path.join(Paths.BIN, SubprocessNames.STUN_CLIENT.value)
    UDP_CONNECTION = os.path.join(Paths.BIN, SubprocessNames.UDP_CONNECTION.value)
    UDP_SERVER = os.path.join(Paths.BIN, SubprocessNames.UDP_SERVER.value)
    UDP_CLIENT = os.path.join(Paths.BIN, SubprocessNames.UDP_CLIENT.value)


class InterprocessMessages(Enum):
    STREAM_REQUEST = "stream_request"
    ACK_STREAM_REQUEST = "ack_stream_request"
    STREAM_ACCEPT = "stream_accept"
    ACK_STREAM_ACCEPT = "ack_stream_accept"
    STREAM_REJECT = "stream_reject"
    ACK_STREAM_REJECT = "ack_stream_reject"

    @staticmethod
    def get_ack(message: "InterprocessMessages") -> "InterprocessMessages":
        try:
            return _ACK_MAP[message]
        except KeyError:
            raise ValueError(f"No acknowledgment defined for {message}")


_ACK_MAP: dict[InterprocessMessages, InterprocessMessages] = {
    InterprocessMessages.STREAM_REQUEST: InterprocessMessages.ACK_STREAM_REQUEST,
    InterprocessMessages.STREAM_ACCEPT: InterprocessMessages.ACK_STREAM_ACCEPT,
    InterprocessMessages.STREAM_REJECT: InterprocessMessages.ACK_STREAM_REJECT,
}
